= 持続可能な自宅サーバー環境を作ろう

== まえがき
さてさて。みなさま思い思いに自宅サーバーを運営なさっていることと思います。
昨今クラウドがもてはやされている中、硬派に自宅にサーバーをおいているアナタ、ステキです。

思いの丈は人それと思いますが、私の場合、クラウドでは何をするにも従量課金になってしまう一方、
自宅サーバーは(電気代及び初期投資を棚に上げれば)自分は計算資源を無料で使えるんだ！と思えるところが魅力的ですね。

ところが、自宅サーバーの運用は簡単ではありません。
よくありがちな問題として、「趣味のサーバーだし、新しく構築するの面倒だからweb用のサーバーだけどゲームサーバーも入れちゃえ！」みたいなことをやっちゃって、
「結局このサーバーって何がはいってるんだっけ？」ってなっちゃったり、
かそもそも手入れのタイミングに間が空きすぎて全部忘れちゃって、同じ内容のサーバーを構築しようと思っても毎回「apache 設定ファイル 場所」みたいなアホな検索をしちゃったり。

自宅サーバーは、放置され、忘れられ、廃墟となっていきがちです。

本稿は、私がこの問題を少しでもマシにするために自宅サーバーを再建した方法を紹介することで、皆様が思い思いの自宅サーバーを構成する「きっかけ」を提供できれば、というコンセプトでお送り致します。
あくまで「一例の紹介」であり、「ベストプラクティスの提案」ではないことをご承知おきください。

== 自宅サーバーはなぜ廃墟化するのか
まえがきで、趣味のサーバーは廃墟になってしまいがちという話をしました。
なぜでしょうか？要因として次が考えられます。

* 用途が気まぐれ 責任範囲が明確でない
* ドキュメントが存在しない
* ヘルスチェックがされていない

=== 用途が気まぐれ 責任範囲が明確でない
なんかOOP(オブジェクト指向プログラミング)で語られがちな言葉がでましたが、
すなわち、いろんなプログラムをインストールしているといつか「このサーバーって何のためにあるんだっけ？」という状態になってしまい、
「このファイルって消して問題ないんだっけ？」「なんかCPU/メモリリソースが足りてないけど何を削ったらいいんだっけ？」という問いを頻発して
メンテナンスを困難にしてしまいます。

解決方法はずばり！単一責任の法則を守るしかないです。
つまり、1つのサーバーに複数のゲームサーバーをインストールしない！これだけです。

しかしこうすると新たに以下の2つの問題が発生します。

A. そんなにマシンがあるわけない
B. ていうかめんどくさい

Aについて、自宅にそんな大量のサーバーマシンを置ける訳はないので(ですよね...?)、1サービス1サーバーみたいな贅沢はできません。
そこで、私はベアメタル・ハイパーバイザーを用いることにしました。
具体的にはVMWareのESXiを使います。なんと無料です！
これはOSレベルの仮想化エンジンで、複数のゲストOSを起動することができます。
これで、1つのマシンで複数のサーバーを起動出来るため、1サービス1サーバーへの道が開けます。

Bについて、これは一見バカみたいな問題に見えますが、趣味においては本質問題です。
たしかに、毎回いちいちESXiの管理画面からcpuやメモリのリソース量を設定して、OSをインストールして...とやるのはめんどくさすぎです。
絶対我慢できずに既存サーバーに別アプリケーションをインストールしちゃいます。
これは、自動化で解決しましょう！
例えば、ちょっとサーバー定義を追加してgithubにpushするだけで自動的にOSと基本ソフトウェアインストール済みのサーバーが上がってくるなら、まだめんどくさがらず運用できる気がしませんか。

=== ドキュメントが存在しない
耳が痛くなる話ですね。
しかし、やはりどのサーバーに何がどうイントールされたのか、どこの設定ファイルをどう編集したのかが分からないと、メンテナンスが困難になってしまいます。
とは言っても！わざわざ趣味で運営してるサーバーのために新しくマニュアル/手順書/ドキュメント、作りたくないものです。

どうしたものか... そういえば一個前の課題の解決方法に、自動化がありました。
ということは！ずばりIaC(Infrastructure as Code)ですね。サーバーの構成やアプリケーションのインストール、configファイルの配布を手動ではなくコードで示すことで、
自動でサーバーが建つ上に・コードそのものがドキュメントとなって一挙両得です。

<!--
-->

<!--
== 本稿で目指す姿
本稿では上の問題を解決するべく、自動でサーバーを構築するサーバーと、モニタリングを行うサーバーを構築しようと思います。
また、ちょっとこれは本筋から離れていて蛇足ではあるのですが、おまけで1台ゲートウェイサーバーを構築します。

というのも、固定IPを自宅に複数引いてる人は稀(ですよね!?)でしょうから、1つのIPでやりくりをする仕組みもあったほうが自宅サーバーが便利になります。
具体的にはapacheでproxyサーバーを立てます。また、最近はteleportっていう踏み台用のミドルウェアが存在しており、

というわけでまとめると、今回作るサーバーは次の3つです。(適当な名前をつけました。やっぱり名前がつくとテンションが上がりますね。)
* 自動構築サーバー BuildersHut
* モニタリングサーバー WatchTower
* ゲートウェイサーバー Bastion

この3つのサーバーがうまーい具合に協力して、新しく構築されたサーバーに対してモニタリングやsshもできる環境を自動で提供できたら、
少ない手間できちんと管理できて使い勝手も良い、持続可能な自宅サーバー環境が作れそうです。

-->

== 自動構築用サーバーを建てよう
本稿では上の問題を解決するべく、自動でサーバーを構築するサーバーを構築しようと思います。


最初の理念通り、作るものはちゃんとコードを残す(=自動構築できるようにする)べきなのです。それは、自動構築サーバーも例に漏れずです。
本章では自動構築用サーバーを作るに当たり私が選定した技術を紹介するとともに、作業の流れを説明します。
また、本章はESXiは構成済みとして話を進めさせて頂きます。

=== サーバーインスタンスを自動で建てる Terraform
なにはともあれ、まずはサーバーインスタンスがなければ始まりません。
TerraformはいわゆるIaC(Infrastucture as Code)を実現するツールの1つです。HashiCorp社(Vagrantとか作ってる)が開発しているOSSで、無料で使えます。 
Terraformは様々なサービスを「Provider」という形で抽象化し、プラグインのように追加できるようにしていることで、同じノリで様々なサービスに対して開発を行う事ができます。 

今回はterraform-provider-esxiという有志の方が作ってくれているproviderを使います。 こうやって広く拡張がコミュニティで開発されているところがTerraformのいいところですね。

ちなみになのですが、Hashicorp社が公式に出しているvsphere-providerというproviderも存在はしています。
ESXiを管理するVSphereというシステムをVMWare社は提供していて、それを通してESXiを構築することができます。
公式なので心惹かれるのですが、ESXiが無料で公開されている一方、VSphereは完全にエンタープライズ向けの製品で、結構良いお値段がするので個人で所有するのはちょっと......という感じですね。 
このproviderで、vsphere抜きにしてESXIだけ触れたら超最高なのですが、そう上手くはいかないですね。

では、terraform-provider-esxiでインスタンスを作ってみましょう。
次のようなHashicorp-HCLを記述します。

main.tf
```HCL
variable "esxi_hostname"     {}
variable "esxi_username"     {}
variable "esxi_password" {}

terraform {
    required_providers {
        esxi = {
            source = "josenk/esxi"
            version = "1.10.2"
        }
    }
}

provider "esxi" {
    esxi_hostname = var.esxi_hostname
    esxi_username = var.esxi_username
    esxi_password = var.esxi_password
}

resource "esxi_guest" "myFirstServer" {

    guest_name     = "MyFirstServer"
    disk_store     = "Disk" = お使いの環境に合わせてください
    virthwver      = "11"   = お使いの環境に合わせてください
    power          = "on"
    boot_disk_size = 32
    guestos        = "ubuntu"
    ovf_source     = "./bionic-server-cloudimg-amd64.ova"

    network_interfaces {
        virtual_network = "main" = お使いの環境に合わせてください
    }
}
```

terraform-provider-esxiはインスタンスの作成だけでなく、OSイメージのOVAファイルをもとにOSのインストールまでやってくれます。
UbuntuのOVAイメージは"https://cloud-images.ubuntu.com/"から見つけることができます。 このmain.tfと同じ階層にダウンロードしておきます。

準備ができたら、"terraform init" -> "terraform plan" -> "terraform apply"と順序立ててコマンドを実行するだけで、あなたのESXiに新しいインスタンスが立っているはずです。

=== OSインストール時に初期設定を行う cloud-init

前章で無事インスタンスをコードから作成することができました。
しかし、ユーザー名やパスワードを設定していないので、sshはできませんし、esxiのclientのコンソールでもログインできません。

この初期設定にcloud-initを使います。

cloud-initは、大体のlinuxディストリビューションのovaイメージ内にすでにインストールしてある、マシンの初期設定を自動で行ってくれるツールです。
shell script形式か、yaml形式で記述することができます。

たとえば、testユーザーを作成するcloud-init.yamlは次のようになります。

```
=cloud-config
users:
  - name: testuser
    plain_text_passwd: "password"
    lock_passwd: false
    sudo: ALL=(ALL) NOPASSWD:ALL
    shell: /bin/bash
    ssh_authorized_keys:
        - ssh-rsa AAAAAAAAAAAAAAA= MYHOSTNAME
```

'users:'というのはcloud-initにおける「モジュール」の1つとなっていて、他にもパッケージをインストールしたり、Diskをマウントしたり、想像以上に多くのことができます。
詳しくは公式ドキュメントの'https://cloudinit.readthedocs.io/en/latest/topics/modules.html'を参考にしてください。


そして、このcloud-initファイルですが、先程のterraformファイルに織り込むことができます。次のとおりです。

main.tf(抜粋)
```
resource "esxi_guest" "myFirstServer" {
    ...
    guestinfo = {
        "userdata"          = base64gzip(file("./cloud-init.yaml"))
        "userdata.encoding" = "gzip+base64"
    }
}
```
ここで指定することで、インスタンスが立ち上がった後自動で実行してくれます。
この後、再度terraform applyすれば自動的にVMが再生成(replace)され、testuserというユーザーが作成された状態で新しいインスタンスが上がってくるはずです。

ところで、サーバーとして使うならIPを固定したいですよね。 ネットワーク関連の設定もcloud-initでできるのですが、少し別枠になります。

別途でmeta.yamlを作成します。
```
network:
  version: 1
  config:
    - type: physical
      name: ens32
      subnets:
        - type: static
          address: 0.0.0.0/0 = 設定したい固定IP
          gateway: 0.0.0.0 = おうちのGW
    - type: nameserver
      address:
        - 0.0.0.0 = おうちのDNS
```

そして、main.tfでmetadataとして読み込みます。

main.tf(抜粋)
```
resource "esxi_guest" "teleport_proxy" {

	...

	guestinfo = {
		"userdata"          = base64gzip(file("./cloud-init.yaml"))
		"userdata.encoding" = "gzip+base64"
		"metadata"          = base64gzip(file("./meta.yaml"))
		"metadata.encoding" = "gzip+base64"
	}
}
```

これで再度terraform applyすれば、また作り変えられた新しいインスタンスが、IPが固定された状態で上がってくはずです。

=== サーバーの中身を設定する Ansible
cloud-initはたしかに便利ではあるものの、これでサーバーのすべてを構築するには少し無理があります。
例えば、cloud-initは外部ファイルを読み込むことができないので、自分でカスタマイズしたconfigファイルをサーバーにプリインストールしようと思うと、
その内容をすべてcloud-initファイルの中にベタ書きする必要があります。これは管理の面からしても厄介です。

一方、cloud-initのようにyaml形式で記述できる構成管理ツールとして、Ansibleというものが存在しています。
Ansibleは結構古くからある構成管理ツールで、次の特徴があります。

==== エージェントレス
エージェントレスというのは、すなわち、構成管理対象に例えば「Ansible-client(存在しません)」のようなものをインストールする必要がないということです。
sshを用いて構成対象に接続し、自動的にコマンドを送ることで構築をする仕組みとなっています。
これは他の構成管理ツールからしても少し珍しい特徴です。

とはいえ、嘘つきと思われないように予め補足しておくと、確かに構築対象にエージェントをインストールする必要はないのですが、とは言え全くなにもインストールしなくても使えるわけではありません。
例えばsshはもちろん必須ですし、他にもpython3が必須となっています。

また、この依存関係は使用するモジュールごとに異なっていて、例えばaptでレポジトリの鍵を新しく登録する'apt_key'モジュールはgpgに依存しています。

モジュールごとに1つ1つ細かく依存関係があり、それらを先にインストールしておかなければ実行途中に失敗してしまうという点がansibleの欠点の1つではありますね。

スムーズにansibleでセットアップできるように、gpgとpython3だけ予めcloud-initでインストールしておくことにします。

cloud-init.yaml(追記)
```
package_update: true
package_upgrade: true
packages:
  - gpg
  - python3
```

==== 冪等性
「冪等性(idempotency)」というのはAnsibleの大きなテーマとなっています。
これは、「何度実行しても結果が同じになる」という意味です。

Ansibleのモジュール1つ1つは、自身に記述してある「あるべき状態」と今の状態を比較し、変更が必要であれば変更し、そうでなければなにもしません。
この挙動により、単なるシェルスクリプトが「手順」にしかなれないのに対し、Ansibleのスクリプトはあるべき状態の「定義」となれるのです。これは、IaCにおける重要な要素の1つです。

ところで、Ansibleそのものは冪等性を保証するものではなく、冪等性を持てるかどうかはモジュールや、それを記述するユーザーに委ねられています。
例えばAnsibleには任意のコマンドを実行できるモジュールがありますが、そのコマンドで何が起こるかまではAnsibleは知ることができません。
ですが、Ansibleは'changed_when'を初めとした様々な便利な指示子により、ユーザーが冪等性を意識し、簡単にそれを実現できるような仕組みを提供しています。

==== 多くのプラグイン
Ansibleは'Ansible Galaxy'と呼ばれる、Ansibleのプラグインを作って共有できるコミュニティがあります。
これにより、例えばApache、様々なRDBMSなどなど、様々なミドルウェアの構成管理が楽になります。

---

Ansibleはここですべて解説するというよりは、「今後登場するミドルウェア」をAnsibleでインストール・設定することになるので、ひとまずの説明はここまでにして、その都度登場してもらいます。

=== 継続的デプロイ ConcourseCI
ここまででコードをもとにTerraformでサーバーインスタンスを建て、Ansibleでミドルウェア等を自動でインストール・設定する術を手に入れました。
次は、これを継続的に自動で実行できるようにしたいですよね。具体的には、Github等に構成管理用のレポジトリを設置し、そこに新たなインスタンス用のコードを追加したら、それをトリガーに自動でレポジトリをPullし、TerraformやAnsibleを自動で実行してほしいです。

それならGithubActionsだ！という感じではありますが、それを使うためにはGithubActionsのワーカーと自宅のネットワークを安全に接続する必要があります。

自宅のLanケーブルをGithubのデータセンターまで持っていって、スイッチにカチッと差し込むわけにはいかないので、別の方法を考えましょう。

CI/CDツールはjenkinsを始め、今となってはたくさん種類がありますが、今回はConcourseCIというものを用いてみました。
理由は見た目がカワイイからです。
以下の特徴があります。

* ワーカーがdockerコンテナで動作する
* 構成がyamlで書ける
* 構成をgitで管理することができる
* gitやs3とのやり取りなど幅広い範囲の入出力がプラグインとして公開されている

concourseはdocker-composeファイルが公開されており、それだけで使い始めることができます。
'https://concourse-ci.org/getting-started.html'

Concourseは様々な種類のプラグインが'resource'として公式・コミュニティ共に多く開発され、公開されており、中にはなんとterraformが動作するものもあります。

私も初めはこれを利用していたのですが、esxiでovaイメージを扱うにはovf-toolが必要で、このresourceには含まれていなかったので自分で作成します。

```Dockerfile
FROM ubuntu:focal

RUN apt update \
 && apt upgrade -y \
 && apt install -y ca-certificates git bash openssh-client curl gpg wget libncursesw5 ansible jq\
 && wget -O- https://apt.releases.hashicorp.com/gpg | gpg --dearmor | \
        tee /usr/share/keyrings/hashicorp-archive-keyring.gpg \
 && echo "deb [signed-by=/usr/share/keyrings/hashicorp-archive-keyring.gpg] https://apt.releases.hashicorp.com focal main" | \
        tee /etc/apt/sources.list.d/hashicorp.list \
 && apt update \
 && apt install -y terraform

RUN mkdir -p $HOME/.ssh
RUN echo "StrictHostKeyChecking no" >> $HOME/.ssh/config
RUN echo "LogLevel quiet" >> $HOME/.ssh/config
RUN chmod 0600 $HOME/.ssh/config

ADD VMware-ovftool-4.3.0-7948156-lin.x86_64.bundle /
RUN chmod +x /VMware-ovftool-4.3.0-7948156-lin.x86_64.bundle \
 && /VMware-ovftool-4.3.0-7948156-lin.x86_64.bundle --console --eulas-agreed --required
```

concourseのファイルは次の通りです。
```
resource_types:
  - name: githubapps-content
    type: docker-image
    source:
      repository: ghcr.io/totegamma/githubapps-content-resource
      tag: master

resources:
  - name: repo-terraform
    type: githubapps-content
    icon: github
    source:
      appID: ((git.appid))
      private_key: ((git.privateKey))
      account: ***
      repository: ***
  - name: ovaimage
    type: s3
    icon: database
    source:
      region_name: ap-northeast-1
      bucket: *** 
      access_key_id: ((s3.access_key))
      secret_access_key: ((s3.secret_key))
      versioned_file: ***/focal-server-cloudimg-amd64.ova
      private: true

jobs:
  - name: terraform-apply
    plan:
    - get: repo-terraform
      trigger: true
    - get: ovaimage
      params:
        url: https://s3.ap-northeast-1.amazonaws.com/***/focal-server-cloudimg-amd64.ova
    - task: terraform-apply
      config:
        platform: linux
        image_resource:
          type: registry-image
          source:
            repository: ***作ったdockerimage***
            tag: main
        inputs:
          - name: repo-terraform
          - name: ovaimage
        params:
          AWS_ACCESS_KEY_ID: ((s3.access_key))
          AWS_SECRET_ACCESS_KEY: ((s3.secret_key))
          AWS_DEFAULT_REGION: 'ap-northeast-1' =terraformのbackendにs3を利用しているため
          esxi_host: ((esxi.host))
          esxi_user: ((esxi.username))
          esxi_password: ((esxi.password))
          ova_filepath: '../ovaimage/focal-server-cloudimg-amd64.ova'
        run:
          path: sh
          args:
            - -ce
            - |
              terraform -chdir=repo-terraform init
              terraform -chdir=repo-terraform apply \
                -var esxi_host="$esxi_host" \
                -var esxi_user="$esxi_user" \
                -var esxi_password="$esxi_password" \
                -var ova_filepath="$ova_filepath" \
                -auto-approve
```

少し長いですね。
入力リソースが2つあることが分かると思います。1つ(repo-terraform)はterraformのレポジトリそのもので、もう1つ(ovaimage)がs3に入っているovaファイルです。
ovaファイルは500MBくらいあるので正直に毎回構築するたびにs3からダウンロードするとちょっとoutgressでお金がかかりそうですが、
Concourseはリソースをいい感じにローカルにキャッシュしてくれるので、恐れず使うことができます。

s3は公式リソースであり、特別な設定なしで利用することができますが、repo-terraformはgithubapps-contentという有志(手前味噌で恐縮ですが私です)が作成したリソースを利用しているため、コードの最初の方にあるresource_typesブロックで定義が必要です。

一番見るべきはコードの最後の方で、terraformを実際にconcourseで実行していることが読み取れると思います。

ですが、実はこのconcourseのyamlファイルは、そのままは読み込むことができません。というのも、コード中のいたるところに((こんな感じの))記法があるのがあるのが分かると思いますが、これは機密情報を別途読み込んでいる部分です。そして、concourseはその本体に機密情報を使う仕組みを持っていないため、他のシークレットマネージャーを用いる必要があります。この設定が行われるまではエラーになっちゃうということです。
Concourseは様々なシークレットマネージャーをサポートしているのですが、私はVaultというものを使いました。

=== 機密情報の管理 Vault

ValutはまたしてもHashicorp社のプロダクトの一つで、例によってオープンソース版は無料で利用することができます。
VaultをインストールするAnsible Playbookは次の通りです。

Ansible
```
- name: Add HashiCorp DEB key
  apt_key:
    url: https://apt.releases.hashicorp.com/gpg
- name: Add HashiCorp repository
  apt_repository:
    repo: deb https://apt.releases.hashicorp.com bionic main
- name: install packages
  apt:
    name:
      - vault
- name: create vault config
  copy:
    src: './vault.hcl'
    dest: '/etc/vault.d/vault.hcl'
- name: ensure that vault is started
  service:
    name: vault
    enabled: true
```

==== Concourse用のシークレットを作成
vaultは鍵をURIのようにスラッシュ区切りの階層構造で表現できます
```
hoge/piyo/fuga/... ← こんな感じ
```
なのですが、この一番最初の'hoge'はそれ以下の空間を代表していて、「データ型」を設定することができます。

hoge以下のpiyoやその下のfuga以降に連なる空間では、hogeのデータ型を継承します。

そして、そのデータ型なのですが、vaultは「kv(key-valueペア)」を始めとして、PKI Certificates, SSH, Transit, TOTPの標準型に加え、クラウドを対象とするもの等 様々な型が定義されています。

concourseでは、concourse/から始まる空間でkv型を使います。

そのため、まずはvaultに、concourseから始まる空間ではkv型を使うと設定します。

```
$ vault secrets enable -version=1 -path concourse kv
Success! Enabled the kv secrets engine at: concourse/
```
これでconcourse/以下に自由にkv型のデータを配置できるようになりました。

ちなみに、concourseの鍵のルックアップルールは、
```
concourse/チーム名/パイプライン名/パラメータ名
```
となっています。んで、若干頭がこんがらがりますが、vaultのkv型ではこの場所にいくつでもkvを定義することができます。
すなわち、concourse/main/test-pipeline/test-parameterに、[{user: hoge}, {pass: piyo}, {shell: bash}...]というふうなデータを入れられるということです。

concourseで((variable))と書いた場合、concourse/<teamname>/<pipelinename>/variableにある、valueという名前のキーが取得されます。 value以外のキー例えばuserを取得したい場合は、((variable.user))と書けばOKです。

KVペアの作成は
```
valut kv put <path> <key>=<value>
```
といったコマンドで可能です。

また、vaultはすごく見た目の良いwebUIが用意されており、そちらで大抵の設定をすることも可能です。併せてご活用ください。

==== Concourse用のロールとtokenの作成
concourseはvaultの全アクセス権を握る必要はなく、concourse/以下の値の読み込みさえできればOKですね。

なので、そのようなポリシーを作成します。
まずポリシー定義ファイルconcourse-policy.hclを以下の内容で作成します。
```
path "concourse/*" {
    capabilities = ["read"]
}
```

次に、作成したconcourse-policy.hclをvalutにインストールします。
```
$ vault policy write concourse ./concourse-policy.hcl
Success! Uploaded policy: concourse
```
これでconcourseポリシーを作ることができました。
では、concourseポリシーに基づいたtokenを発行します。
```
ansible@BuildersHut:~$ vault token create --policy concourse
```

これにより、concourse/以下の値の読み込みだけが出来るトークンを作ることができました。

このトークンをconcourseCIに設定します！docker-composeファイルに以下を追記するだけです。
```
CONCOURSE_VAULT_URL: http://<よしなに-ipv4>:8200
CONCOURSE_VAULT_CLIENT_TOKEN: hvs.*******************************************************************************************
```

=== 自動構築用サーバー完成！
これで、レポジトリに構成の定義をpushするだけで、好きにサーバーを構築できるようになりました。
これにより、楽に1サーバーが負う責任範囲を限定し、メンテナンスしやすく、かつIaCによりドキュメントも残る、
持続可能なサーバー環境に一歩近づきました。

== おまけ: 他にも便利な管理用サーバーを構築しよう！
=== モニタリングサーバー
いつの間にかディスク容量がfullになってて、何かおかしいな〜と思ってsshしたらbashに「bash: cannot create temp file for here-document: No space left on device」って言われて冷や汗出た経験、
あるんじゃないでしょうか。
やはり、趣味のサーバーとはいえモニタリングの存在は重要です。

私はPrometheusとGrafanaの鉄板コンビで構築しました。
ところで、今回インスタンスの自動生成をやっているので、新しくできたインスタンスをもちろん自動的に監視対象に加えたいですよね。
prometheusは監視対象を柔軟に決定するための、ServiceDiscoveryの機能が搭載されています。

ここでは、一番シンプルなfile_sdを使うことにします。
file_sdは正直ServiceDiscoveryと呼ぶには微妙なのですが、要するに別ファイルに記載したターゲットの一覧を、定期的に読み込んでくれるといったものです。

今回、監視対象となるAnsibleに登録されている変数をもとに生成するのが楽ですね。
Ansibleは内部にjinja2と呼ばれるpythonのテンプレートエンジンが搭載されており、
変数をもとにファイルを生成することができます。

```yaml
- name: template
  template:
    src: targets.j2
    dest: /etc/targets.yml
  register: config_updated
```

```j2
- targets:
{% for elem in groups.All %}
  - {{elem}}:9100
{% endfor %}
```

=== ゲートウェイサーバー
固定IPを自宅に複数引いてる人は稀(ですよね!?)でしょうから、1つのIPでやりくりをする仕組みがあると嬉しいですよね。
apacheでproxyサーバーを建てましょう。

以下のように設定できます。
```
<VirtualHost *:80>
  RewriteEngine on
  RewriteCond %{HTTPS} off
  RewriteRule ^(.*)$ https://%{HTTP_HOST}%{REQUEST_URI} [R=301,L]
</VirtualHost>

<VirtualHost *:443>
  ServerName myservice.example.com
  ProxyPass / http://myserviceip:port/
  ProxyPassReverse / http://myserviceip:port/
  SSLEngine on
  SSLCertificateFile /etc/letsencrypt/live/example.com/fullchain.pem
  SSLCertificateKeyFile /etc/letsencrypt/live/example.com/privkey.pem
</VirtualHost>
```

grafanaとteleportはhttp(s)だけでなく、ws(s)も通す必要があります。
これは、RewriteConfigで実現できます。
```
RewriteEngine on
RewriteCond %{HTTP:Upgrade}    websocket        [NC]
RewriteCond %{HTTP:Connection} upgrade          [NC]
RewriteRule ^/?(.*) "ws://myserviceip/$1"  [P]
```

上記の設定で証明書を指定してhttps対応していますが、証明書の取得はCertbotを使って
無料でLet's Encryptから取得できます。

ワイルドカード証明書が欲しい場合は、ドメインに特定のレコードを追加する、少し難しいチャレンジをクリアする必要がありますが、ドメインをAWSのRoute53で管理している場合、dns-route53というプラグインを使うことで、自動で行うことができます。

また、最近はteleportという踏み台用のミドルウェアが存在しており、これを使うとWebUIからターミナルに接続することができます。
サーバーごとにsshのポートを変えたり、いちいち踏み台サーバー越しに接続するのは面倒なので、これも便利です。

== おわりに
本稿では、自宅サーバーを持続可能にする一つの解決策として、自動構築サーバーを作ることと、その作り方の一例の紹介をしました。
また、一緒に構築するとより自宅サーバーを便利に活用できるサーバーも併せて紹介しました。

自動構築は非常に楽しいですね！カードゲームでオレオレコンボデッキを組んでいるような感じです。
みんなもぜひ自宅サーバーのデッキ構築してみて、そして紹介してください！


