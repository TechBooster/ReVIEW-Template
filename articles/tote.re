= 持続可能な自宅サーバー環境を作ろう

== まえがき
さてさて。みなさま思い思いに自宅サーバーを運用なさっていることと思います。
昨今クラウドがもてはやされている中、硬派に自宅にサーバーをおいているアナタ、ステキです。

思いの丈は人それと思いますが、筆者の場合、クラウドでは何をするにも従量課金になってしまう一方、
自宅サーバーは(電気代及び初期投資を棚に上げれば)自分は計算資源を無料で使えるんだ！と思えるところが魅力的ですね。

ところで、自宅サーバーの運用は簡単ではありません。
ありがちな問題として、「趣味のサーバーだし、新しく構築するの面倒だからweb用のサーバーだけどゲームサーバーも入れちゃえ！」みたいなことをやってしまい、
「結局このサーバーって何がはいってるんだっけ？」ってなってしまったり、
かそもそも手入れのタイミングに間が空きすぎて全部忘れてしまい、同じ内容のサーバーを構築しようと思っても毎回「apache 設定ファイル 場所」みたいな検索をしちゃったりしたことは、誰しも1度はあるのではないでしょうか。

@<b>{自宅サーバーは、放置され、忘れられ、廃墟となっていきがち}です。

本稿は、筆者の自宅にて朽ち果てたサーバーをリノベーションするにあたり、今後こういったこの問題の発生を少しでもマシにするためにとった方法を紹介することで、
皆様が思い思いの自宅サーバーを構成する「きっかけ」を提供できれば、というコンセプトでお送り致します。
あくまで「一例の紹介」であり、「ベストプラクティスの提案」ではないことをご承知おきください。

== 自宅サーバーはなぜ廃墟化するのか
まえがきで、趣味のサーバーは廃墟になってしまいがちという話をしました。
なぜでしょうか？要因として次が考えられます。

=== 原因1. サーバーの用途が気まぐれ 責任範囲が明確でない
OOP(オブジェクト指向プログラミング)で語られがちな言葉がでましたが、
すなわち、いろんなプログラムをインストールしているといつか「このサーバーって何のためにあるんだっけ？」という状態になってしまい、
「このファイルって消して問題ないんだっけ？」「なんかCPU/メモリリソースが足りてないけど何を削ったらいいんだっけ？」という問いを頻発して
メンテナンスを困難にしてしまいます。

解決方法はずばり、単一責任の法則を守ることです。
すなわち@<b>{1つのサーバーに複数のサービスを展開しないこと！}これだけです。

しかしこれを守るには新たに以下の2つの問題があります。

 * A. そんなにマシンが家にあるわけない
 * B. ていうかめんどくさい

Aについて、たしかに自宅にそんな大量のサーバーマシンを置ける訳はないので(ですよね...?)、1サービス1サーバーみたいな贅沢はできません。
そこで、筆者はベアメタル・ハイパーバイザーを用いることにしました。
具体的にはVMWareのESXiを使います。なんと無料です！
これはOSレベルの仮想化エンジンで、複数のゲストOSを起動することができます。
これで、1つのマシンで複数のサーバーを起動出来るため、1サービス1サーバーへの道が開けます。

Bについて、これは一見バカみたいな問題に見えますが、趣味においては@<b>{本質問題}であり、@<b>{一番の原因}です。
たしかに、毎回いちいちESXiの管理画面からcpuやメモリのリソース量を設定して、OSをインストールして...とやるのはめんどくさすぎです。
筆者は絶対我慢できずに既存サーバーに別アプリケーションをインストールしてしまいます。

これは、自動化で解決しましょう！
例えば、ちょっとサーバー定義を追加してgithubにpushするだけで自動的にOSと基本ソフトウェアインストール済みのサーバーが上がってくるなら、
まだめんどくさがらず運用できる気がしませんか。

=== 原因2. ドキュメントが存在しない
耳が痛くなる話です。

しかし、やはりどのサーバーに何がどうイントールされたのか、どこの設定ファイルをどう編集したのかが分からないと、メンテナンスが困難になってしまいます。
とは言っても！わざわざ趣味で運営してるサーバーのために新しくマニュアル/手順書/ドキュメント、作りたくないものです。

どうしたものか......。 そういえば一個前の課題の解決方法に、自動化がありました。
ということは！ずばりIaC(Infrastructure as Code)ですね。サーバーの構成やアプリケーションのインストール、configファイルの配布を手動ではなくコードで示すことで、
自動でサーバーが建つ上に・コードそのものがドキュメントとなって一挙両得です。


== 自動構築用サーバーを建てよう
本稿では前述の問題を解決するべく、ハイパーバイザーを導入し、さらに自動でサーバーを構築するサーバーを構築しようと思います。

そして最初の理念通り、作るものはちゃんとコードを残す(=自動構築できるようにする)べきなのです。それは、自動構築サーバーも例に漏れずです。
本章では自動構築用サーバーを作るに当たり筆者が選定した技術を紹介するとともに、作業の流れを説明します。

また、ESXiの導入方法は通常ののOSのインストールのようなもので特段筆者から話すことがないため、以降の章ではESXi本体はマシンに展開済みとして進めさせて頂きます。

=== サーバーインスタンスを自動で建てる Terraform
なにはともあれ、まずはサーバーインスタンスがなければ始まりません。
TerraformはいわゆるIaC(Infrastucture as Code)を実現するツールの1つです。HashiCorp社(他の製品ではVagrantなどが有名ですね)が開発しているOSSで、無料で使えます。 
Terraformは例えばAWSやGCPを始めとしたクラウドサービスを初めとした様々なサービスに対するインターフェイスを「Provider」という形で抽象化し、
プラグインのように追加できるようにしていることで、同じような記法で様々なサービスに対して開発を行う事ができます。 

また、Terraformはファイル本体に利用するProviderを指定するだけで、@<code>{terraform init}コマンドを実行した際に必要なProviderをダウンロードしてきてくれるので、依存関係などを自分で解消する必要がないのが便利です。

今回はESXiに対して構築を行いたい為、terraform-provider-esxiという有志の方が作ってくれているproviderを使います。
こうやって広く拡張がコミュニティで開発されているところがTerraformのいいところですね。

//notice[注意]{
基本的にはprovider自身の依存関係を気にする必要はないのですが、今回使うterraform-provider-esxiはovftoolsという
vmware社のツールに依存しているため、それは予めホストマシンにインストールしておく必要があります。
//}

====[column] コラム

ちなみに、Hashicorp社が公式に出しているvsphere-providerというproviderも存在はしています。
ESXiを管理するVSphereというシステムをVMWare社は提供していて、それを通してESXiを構築することができます。
公式なので心惹かれるのですが、ESXiが無料で公開されている一方、VSphereは完全にエンタープライズ向けの製品で、結構良いお値段がするので個人で所有するのはちょっと......という感じですね。 
このproviderで、vsphere抜きにしてESXIだけ触れたら超最高なのですが、そう上手くはいかないですね。

====[/column]

では、terraform-provider-esxiで構築用サーバーのインスタンスを作ってみましょう。
次のようなHashicorp-HCLを記述します。

//list[tote-main-tf-1][main.tf][hcl]{
variable "esxi_hostname"     {}
variable "esxi_username"     {}
variable "esxi_password"     {}

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
    disk_store     = "Disk" # お使いの環境に合わせてください
    virthwver      = "11"   # お使いの環境に合わせてください
    power          = "on"
    boot_disk_size = 32
    guestos        = "ubuntu"
    ovf_source     = "./bionic-server-cloudimg-amd64.ova"

    network_interfaces {
        virtual_network = "main" # お使いの環境に合わせてください
    }
}
//}

terraform-provider-esxiはインスタンスの作成だけでなく、OSイメージのOVAファイルをもとにOSのインストールまでやってくれます。
UbuntuのOVAイメージは@<code>{https://cloud-images.ubuntu.com/}から見つけることができます。 このmain.tfと同じ階層にダウンロードしておきます。

準備ができたら、@<code>{"terraform init" -> "terraform plan" -> "terraform apply"}と順序立ててコマンドを実行するだけで、
あなたのESXiに新しいインスタンスが立っているはずです。

=== OSインストール時に初期設定を行う cloud-init

前章で無事インスタンスをコードから作成することができました。
しかし、ユーザー名やパスワードを設定していないので、sshはできませんし、esxiのclientのコンソールでもログインできません。

この初期設定にcloud-initを使います。

cloud-initは、大体のlinuxディストリビューションのovaイメージ内にすでにインストールしてある、マシンの初期設定を自動で行ってくれるツールです。
shell script形式か、yaml形式で記述することができます。

たとえば、testユーザーを作成するcloud-init.yamlは次のようになります。

//list[tote-cloud-init][cloud-init.yaml][yaml]{
#cloud-config
users:
  - name: testuser
    plain_text_passwd: "password"
    lock_passwd: false
    sudo: ALL=(ALL) NOPASSWD:ALL
    shell: /bin/bash
    ssh_authorized_keys:
        - ssh-rsa AAAAAAAAAAAAAAA= MYHOSTNAME
//}

@<code>{users:}というのはcloud-initにおける「モジュール」の1つとなっていて、他にもパッケージをインストールしたり、Diskをマウントしたり、想像以上に多くのことができます。
詳しくは公式ドキュメントの@<code>{https://cloudinit.readthedocs.io/en/latest/topics/modules.html}を参考にしてください。

そして、このcloud-initを、先程のterraformコード内に読み込ませることで、サーバーインスタンスが立ち上がった後、自動でこれを実行させることができます。

//list[tote-main-tf-2][main.tf(抜粋)][hcl]{
resource "esxi_guest" "myFirstServer" {
    ...
    guestinfo = {
        "userdata"          = base64gzip(file("./cloud-init.yaml"))
        "userdata.encoding" = "gzip+base64"
    }
}
//}

この後、再度terraform applyすれば自動的にVMが再生成(replace)され、testuserというユーザーが作成された状態で新しいインスタンスが上がってくるはずです。

//info[ちなみに...]{
terraformは基本的には「現状」と「コード上の定義」を見比べて、変更が必要なところだけ変更をかけてくれます。
ですが、例えば仮想ディスクの容量など、「作成後変更できない」パラメーターも中には存在します。

ここで使っているcloud-initの設定項目もこのパターンになるので、強制的にサーバーの立て直しが行われるというわけです。
//}

ところで、サーバーとして使うならIPを固定したいですよね。 ネットワーク関連の設定もcloud-initでできるのですが、少し別枠になります。

新しくmeta.yamlを作成します。
//list[tote-cloud-init-1][meta.yaml][yaml]{
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
//}

そして、main.tfでmetadataとして読み込みます。

//list[tote-main-tf-3][main.tf(抜粋)][hcl]{
resource "esxi_guest" "teleport_proxy" {
	...
	guestinfo = {
		"userdata"          = base64gzip(file("./cloud-init.yaml"))
		"userdata.encoding" = "gzip+base64"
		"metadata"          = base64gzip(file("./meta.yaml"))
		"metadata.encoding" = "gzip+base64"
	}
}
//}

これで再度terraform applyすれば、また作り変えられた新しいインスタンスが、IPが固定された状態で上がってくはずです。

====[column] コラム

本文中では触れていませんが、筆者は@<code>{esxi_virtual_disk}リソースを利用して、追加のディスクを作成し、@<code>{esxi_guest}の@<code>{virtual_disks}で追加で指定することで、
インスタンスに2つのディスクを付与しています。というのも、インスタンスを設定変更ではなく再構築した場合(replaceがかかった場合)、サーバー上のデータが消えてしまうからです。

設定データなどはgit管理とシンクしている状態が正なので消えて良いのですが、例えばゲームサーバーであればセーブデータは消えてもらっては困りますし、とは言えセーブデータをgitに上げるのはちょっと違いますよね。

なので、2作ったディスクのうち、1つはOSなどがインストールされている「消えて良いディスク」、もう1つがdbのデータなどが入っている「消えたら困るディスク」として運用しています。

ちなみに、diskのセットアップはcloud-initで次のように設定できます。

//list[tote-disk-setup][cloud-init.yaml][yaml]{
device_aliases:
    persistent: /dev/sdb

disk_setup:
    persistent:
        table_type: gpt
        layout: [100]

fs_setup:
    - label: fs1
      filesystem: ext4
      device: persistent.1

mounts:
    - ["persistent.1", "/persistent"]
//}

====[/column]

=== サーバーの中身を設定する Ansible
cloud-initはたしかに便利ではあるものの、これでサーバーのすべてを構築するには少し無理があります。
例えば、cloud-initは外部ファイルを読み込むことができないので、自分でカスタマイズしたconfigファイルをサーバーに配置しようと思うと、
その内容をすべてcloud-initファイルの中にベタ書きする必要があります。これは管理の面からしても厄介です。

一方、cloud-initのようにyaml形式で記述できる構成管理ツールとして、Ansibleというものが存在しています。
Ansibleは結構古くからある構成管理ツールで、次の特徴があります。

==== エージェントレス
エージェントレスというのは、すなわち、構成管理対象に例えば「Ansible-client(実在しません)」のようなものをインストールする必要がないということです。
sshを用いて構成対象に接続し、自動的にコマンドを送ることで構築をする仕組みとなっています。
これは他の構成管理ツールからしても少し珍しい特徴です。

とはいえ、嘘つきと思われないように予め補足しておくと、確かに構築対象にエージェントをインストールする必要はないのですが、とは言え全くなにもインストールしなくても使えるわけではありません。
例えばssh-serverはもちろん必須ですし、他にもpython3が必須となっています。

また、この依存関係は使用するモジュールごとに異なっていて、例えばaptでレポジトリの鍵を新しく登録する@<code>{apt_key}モジュールは@<code>{gpg}に依存しています。

モジュールごとに1つ1つ細かく依存関係があり、それらを先にインストールしておかなければ実行途中に失敗してしまうという点がansibleの欠点の1つではありますね。

スムーズにansibleでセットアップできるように、gpgとpython3だけ予めcloud-initでインストールしておくことにします。

//list[tote-cloud-init-2][cloud-init.yaml(追記)][yaml]{
package_update: true
package_upgrade: true
packages:
  - gpg
  - python3
//}

==== 冪等性
「冪等性(idempotency)」というのはAnsibleの大きなテーマとなっています。
これは、「何度実行しても結果が同じになる」という意味です。

Ansibleのモジュール1つ1つは、自身に記述してある「あるべき状態」と今の状態を比較し、変更が必要であれば変更し、そうでなければなにもしません。
この挙動により、単なるシェルスクリプトが「手順」にしかなれないのに対し、Ansibleのスクリプトはあるべき状態の「定義」となれるのです。これは、IaCにおける重要な要素の1つです。

ところで、Ansibleそのものは冪等性を保証するものではなく、冪等性を持てるかどうかはモジュールや、それを記述するユーザーに委ねられています。
例えばAnsibleには任意のコマンドを実行できるモジュールがありますが、そのコマンドで何が起こるかまではAnsibleは知ることができません。
ですが、Ansibleは@<code>{changed_when}を初めとした様々な便利な指示子により、ユーザーが冪等性を意識し、簡単にそれを実現できるような仕組みを提供しています。

==== 多くのプラグイン
Ansibleは@<code>{Ansible Galaxy}と呼ばれる、Ansibleのプラグインを作って共有できるコミュニティがあります。
これにより、例えばApacheや種々のRDBMSなどなど、様々なミドルウェアの構成管理が楽になります。

====[column] コラム
Ansibleは、構築対象のサーバーをリストアップした「inventory」と呼ばれるファイルを用いて対象を管理します。
ところで、今はサーバーを自動で増減できるシステムを作っているので、当然このinventoryファイルも自動で生成したいですよね。

terraformは、showコマンドを用いて、stateファイルをもとに現在構成されているすべてのリソースに関する情報をjsonで吐き出すことができます。
Ansibleはinveontoryファイルの形式としてjsonが使えないのですが、yamlが使えるので、一度jqで必要な要素だけ取り出し、整形し、yqでyamlに変換するスクリプトを書いておくと便利です。これは、後にCI/CDプロセスに入れ込みます。

//cmd{
terraform -chdir=repo-terraform init
terraform -chdir=repo-terraform show -json \
  | jq '.values.root_module.resources
    | map(select(.type == "esxi_guest")) | {
      "all": {
        "children": (
          map({(.name): {"hosts": {(.values.ip_address): {}}}}) + [
            {
              "All": {
                "children": map({
                  (.name): {}
                }) | add
              }
            }
          ]
        ) | add
      }
    }' \
  | yq -p j -o y '.' > inventory/host
//}

このスクリプトでは、それぞれのホスト名をグループ名にした、単一のIPアドレスからなるグループ群に加え、すべてのホストが入っているALLというグループを作っています。

====[/column]

=== 継続的デプロイ ConcourseCI
ここまでで、コードをもとにTerraformでサーバーインスタンスを建て、Ansibleでミドルウェア等を自動でインストール・設定する術を手に入れました。
次は、これを継続的に自動で実行できるようにしたいですよね。具体的には、Github等に構成管理用のレポジトリを設置し、そこに新たなインスタンス用のコードを追加したら、それをトリガーに自動でレポジトリをPullし、TerraformやAnsibleを自動で実行してほしいです。

それならGithubActionsだ！という感じではありますが、それを使うためにはGithubActionsのワーカーと自宅のネットワークを安全に接続する必要があります。

自宅のLanケーブルをGithubのデータセンターまで持っていって、スイッチにカチッと差し込むわけにはいかないので、別の方法を考えましょう。

CI/CDツールはjenkinsを始め、今となってはたくさん種類がありますが、今回はConcourseCIというものを用いてみました。
理由は見た目がカワイイからです。



他にも次の特徴があります。

 * ワーカーがdockerコンテナで動作する
 * 構成がyamlで書ける
 * 構成をgitで管理することができる
 * gitやs3とのやり取りなど幅広い範囲の入出力がプラグインとして公開されている

concourseはdocker-composeファイルが公開されており、それだけで使い始めることができます。
@<code>{https://concourse-ci.org/getting-started.html}

Concourseは様々な種類のプラグインが@<code>{resource}として公式・コミュニティ共に多く開発され、公開されており、中にはなんとterraformが動作するものもあります。

筆者も初めはこれを利用していたのですが、esxiでovaイメージを扱うにはovf-toolが必要で、このresourceには含まれていません。

仕方がないのでresourceではなくスクリプト本体でterraformを実行しようと思いますが、毎度ワーカーにterraformをインストールしていては実行が遅くなってしまうので、
先に今回タスクを実行するためのdockerファイルを手元で作成し、レジストリに登録しておきます。

//list[tote-dockerfile][Dockerfile][dockerfile]{
FROM ubuntu:bionic

RUN apt update \
 && apt upgrade -y \
 && apt install -y ca-certificates git bash openssh-client \
                   curl gpg wget libncursesw5 ansible jq\
 && wget -O- https://apt.releases.hashicorp.com/gpg | gpg --dearmor | \
        tee /usr/share/keyrings/hashicorp-archive-keyring.gpg \
 && echo "deb [signed-by=/usr/share/keyrings/hashicorp-archive-keyring.gpg] \
              https://apt.releases.hashicorp.com bionic main" | \
        tee /etc/apt/sources.list.d/hashicorp.list \
 && apt update \
 && apt install -y terraform

RUN mkdir -p $HOME/.ssh
RUN echo "StrictHostKeyChecking no" >> $HOME/.ssh/config
RUN echo "LogLevel quiet" >> $HOME/.ssh/config
RUN chmod 0600 $HOME/.ssh/config

ADD VMware-ovftool-4.3.0-7948156-lin.x86_64.bundle /
RUN chmod +x /VMware-ovftool-4.3.0-7948156-lin.x86_64.bundle \
 && /VMware-ovftool-4.3.0-7948156-lin.x86_64.bundle \
        --console --eulas-agreed --required
//}

では、作成したdocker imageを使いつつ、terraformを実行するconcourseのjobファイルを作成しましょう。
//list[tote-concourse-yaml][concourse-task.yaml][yaml]{
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
      versioned_file: ***/***-cloudimg-amd64.ova
      private: true

jobs:
  - name: terraform-apply
    plan:
    - get: repo-terraform
      trigger: true
    - get: ovaimage
      params:
        url: https://s3.ap-northeast-1.amazonaws.com/***/***-cloudimg-amd64.ova
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
          AWS_DEFAULT_REGION: 'ap-northeast-1'
          esxi_host: ((esxi.host))
          esxi_user: ((esxi.username))
          esxi_password: ((esxi.password))
          ova_filepath: '../ovaimage/***-cloudimg-amd64.ova'
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
//}

少し長いですね。
入力リソースが2つあることが分かると思います。1つ(repo-terraform)はterraformのレポジトリそのもので、もう1つ(ovaimage)がs3に入っているovaファイルです。
ovaファイルは500MBくらいあるので構築するたびにs3からダウンロードするとoutgressの料金がかさみそうですが、
実際はConcourseはリソースをローカルにキャッシュしてくれるので、恐れず使うことができます。

s3は公式リソースであり、特別な設定なしで利用することができますが、repo-terraformはgithubapps-contentという有志(手前味噌で恐縮ですが筆者です)が作成したリソースを利用しているため@<fn>{tote-githubapp-content}、コードの最初の方にあるresource_typesブロックで定義が必要です。

一番見るべきはコードの最後の方で、terraformを実際にconcourseで実行していることが読み取れると思います。

次に、この処理が完了した後にansibleを実行するようにしましょう。

//footnote[tote-githubapp-content][concourseには標準でgitリソースが存在しているのですが、これは個人のssh鍵を利用するする必要があります。筆者は特定のOrganizationに限ってcloneできる鍵を作りたかったため、GithubAppsの認証情報を用いてcloneできるようにしたリソース、githubapp-contentを作成したという背景があります。個人的にもおすすめしています。 https://github.com/totegamma/githubapps-content-resource]

//list[tote-concourse-yaml2][ansible実行箇所][yaml]{
- name: ansible
  plan:
  - get: repo-terraform
    passed: [terraform-apply]
    trigger: true
  - get: repo-ansible
    trigger: true
  - task: ansible
    config:
      platform: linux
      image_resource:
        type: registry-image
        source:
          repository: ***
          tag: main
      inputs:
        - name: repo-ansible
      params:
        ANSIBLE_KEY: ((ansible.private_key))
      run:
        path: sh
        args:
          - -ce
          - |
            cd repo-ansible
            echo "$ANSIBLE_KEY" > ansible_key
            chmod 700 ansible_key
            TARGETS=$(cat host | yq '.all.children.All.children \
                               | keys | join(" ")')
            for TARGET in $TARGETS; do
                echo $TARGET
                if [ -d $TARGET ]; then
                  ansible-playbook -i host \
                        --private-key ansible_key $TARGET/playbook.yaml
                else
                  cp common/default.yaml $TARGET.yaml
                  sed -i -e "s/<TARGET>/$TARGET/g" $TARGET.yaml
                  ansible-playbook -i host --private-key ansible_key $TARGET.yaml
                fi
            done
//}

特にterraformの実行がansibleの実行になっただけで特に変わりないですが、少しシェルスクリプトで複雑なことをしています。
これは、①管理対象の全てにAnsibleをそれぞれ実行する という意味合いと、②管理対象のホスト名のAnsible-playbookがレポジトリに存在すればそれをそのまま実行し、そうでなければデフォルトのplaybookを実行する　という意味合いでできています。

この辺はつくる人それぞれのカスタマイズが生きる箇所だと思いますが、筆者は「自動構築で管理するサーバーはそれはそれとして、ちょっと特定のミドルウェアの検証だけで作るサーバーはも基本的な設定を施した上で自動構築したい。」という気持ちがあったのでこのようなスクリプトを書きました。

====[column] コラム

筆者は上記のterraformを実行するjob、ansibleを実行するjobに加え、Discordコマンドをイベントとして受け取り、特定のリソースをtaint状態にするjobを追加しています。
taint状態はterraformのリソースが取りうる状態の一つで、この状態に指定されたリソースは、次のapply時に強制的にreplaceがかかります。

taint状態などはgit上のファイルに直接記述するようなものではなく、イベント的に発生する事項なのでこのような実装にしています。
詳しくは、筆者のブログ( https://gammalab.net/blog/pdjvbkb94na2p/ )も併せて御覧ください。

====[/column]

それはそれとして、実はこのconcourseのyamlファイルは、そのままは読み込むことができません。というのも、コード中のいたるところに((こんな感じの))記法があるのにお気づきだと思いますが、これは機密情報をファイルの外から読み込んでいる部分です。concourseはその本体に機密情報を使う仕組みを持っていないため、他のシークレットマネージャーを用いる必要がありますが、この設定が行われるまではエラーになってしまいます。
Concourseは様々なシークレットマネージャーをサポートしているのですが、筆者はVaultを採用しました。

=== 機密情報の管理 Vault

ValutはまたしてもHashicorp社のプロダクトの一つで、例によってオープンソース版は無料で利用することができます。
VaultをインストールするAnsible Playbookは次の通りです。

//list[tote-vault-ansible][ansible][yaml]{
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
//}

==== Concourse用のシークレットを作成
vaultは鍵をURIのようにスラッシュ区切りの階層構造で表現できます
//cmd{
hoge/piyo/fuga/... ← こんな感じ
//}
ところが、この一番上位にある空間である'hoge'はそれ以下の空間を代表していて、「データ型」を設定することができます。
また、hoge以下のpiyoやその下のfuga以降に連なる空間では、hogeのデータ型を継承します。

vaultにおけるデータ型は「kv(key-valueペア)」を始めとして、PKI Certificates, SSH, Transit, TOTPの標準型に加え、クラウドを対象とするもの等 様々な型が定義されています。

concourseでは、concourse/から始まる空間でkv型を使います。
そのため、まずはvaultに、concourseから始まる空間ではkv型を使うと設定します。

//cmd{
$ vault secrets enable -version=1 -path concourse kv
Success! Enabled the kv secrets engine at: concourse/
//}
これでconcourse/以下に自由にkv型のデータを配置できるようになりました。

ちなみに、concourseの鍵のルックアップルールは、
//cmd{
concourse/チーム名/パイプライン名/パラメータ名
//}
となっています。そして、vaultのkv型ではこの空間に複数のkvを定義することができます。
すなわち、concourse/main/test-pipeline/test-parameterに、@<code>{[{user: hoge}, {pass: piyo}, {shell: bash}...]}というなデータを入れられるということです。

concourseで((variable))と書いた場合、concourse/<teamname>/<pipelinename>/variableにある、valueという名前のキーが取得されます。 value以外のキー例えばuserを取得したい場合は、((variable.user))と書けばOKです。

KVペアの作成は
//cmd{
$ valut kv put <path> <key>=<value>
//}
のように、kvコマンドを用いて作成できます。

//info[ちなみに...]{
今回はコマンドベースでの説明を行いましたが、vaultはすごく見た目の良いwebUIが用意されており、そちらで大抵の設定をすることも可能です。併せてご活用ください。
//}

==== Concourse用のロールとtokenの作成
concourseはvaultの全アクセス権を握る必要はなく、concourse/以下の値の読み込みさえできれば十分です。

なので、そのようなポリシーを作成します。
まずポリシー定義ファイルconcourse-policy.hclを以下の内容で作成します。

//list[tote-concourse-policy][concourse-policy.hcl][hcl]{
path "concourse/*" {
    capabilities = ["read"]
}
//}

次に、作成したconcourse-policy.hclをvalutにインストールします。
//cmd{
$ vault policy write concourse ./concourse-policy.hcl
Success! Uploaded policy: concourse
//}
これでconcourseポリシーを作ることができました。
では、concourseポリシーに基づいたtokenを発行します。
//cmd{
ansible@BuildersHut:~$ vault token create --policy concourse
//}

これにより、concourse/以下の値の読み込みだけが出来るトークンを作ることができました。

このトークンをconcourseCIに設定します！docker-composeファイルに以下を追記するだけです。
//cmd{
CONCOURSE_VAULT_URL: http://<myip>:8200
CONCOURSE_VAULT_CLIENT_TOKEN: hvs.****************
//}

これで、Concourseから安全にsshの鍵などを利用できるようになり、前述していたterraformやansibleの自動化タスクがデプロイできるようになりました。

=== 自動構築用サーバー完成！
これで、レポジトリに構成の定義をpushするだけで、好きにサーバーを構築できるようになりました。
これにより、楽に1サーバーが負う責任範囲を限定し、メンテナンスしやすく、かつIaCによりドキュメントも残る、
持続可能なサーバー環境に一歩近づいたのではないでしょうか。

== おまけ: 他にも便利な管理用サーバーを構築しよう！
自動構築サーバーができましたが、次はこれを使って何を建てましょうか。
早速minecraftのゲームサーバーを建てる...... のも良いのですが、他にも作っておくとQOLが爆上がりするサーバーが2種あるので、コラム程度に簡単にご紹介します。

=== モニタリングサーバー
いつの間にかディスク容量がfullになってて、何かおかしいな〜と思ってsshしたらbashに「bash: cannot create temp file for here-document: No space left on device」って言われて冷や汗出た経験、一度はあるのではないでしょうか。
やはり、趣味のサーバーとはいえモニタリングの存在は重要です。

筆者はPrometheusとGrafanaの鉄板コンビで構築しました。
ところで、今回インスタンスの自動生成をやっているので、新しくできたインスタンスをもちろん自動的に監視対象に加えたいですよね。
prometheusは監視対象を柔軟に決定するための、ServiceDiscoveryの機能が搭載されています。

ここでは、一番シンプルなfile_sdを使うことにします。
file_sdは正直ServiceDiscoveryと呼ぶには微妙なのですが、要するに別ファイルに記載したターゲットの一覧を、定期的に読み込んでくれるといったものです。

今回、監視対象は、Ansibleに登録されている変数をもとに生成するのが楽ですね。
Ansibleは内部にjinja2と呼ばれるpythonのテンプレートエンジンが搭載されており、
変数をもとにファイルを生成することができます。

//list[tote-call-template][テンプレートの呼び出し][yaml]{
- name: template
  template:
    src: targets.j2
    dest: /etc/targets.yml
  register: config_updated
//}

//list[tote-template][targets.j2][j2]{
- targets:
{% for elem in groups.All %}
  - {{elem}}:9100
{% endfor %}
//}

=== ゲートウェイサーバー
固定IPを自宅に複数引いてる人は稀(ですよね!?)でしょうから、1つのIPでやりくりをする仕組みがあると嬉しいですよね。
apacheでproxyサーバーを建てましょう。

以下のように設定できます。

//list[tote-apache-conf][apache.conf][conf]{
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
//}

grafanaとteleportはhttp(s)だけでなく、ws(s)も通す必要があります。
これは、RewriteConfigで実現できます。

//list[tote-apache-conf][websocketのプロキシ][conf]{
RewriteEngine on
RewriteCond %{HTTP:Upgrade}    websocket   [NC]
RewriteCond %{HTTP:Connection} upgrade     [NC]
RewriteRule ^/?(.*) "ws://myserviceip/$1"  [P]
//}

上記の設定で証明書を指定してhttps対応していますが、証明書の取得はCertbotを使って
無料でLet's Encryptから取得できます。

ワイルドカード証明書が欲しい場合は、ドメインに特定のレコードを追加する、少し難しいチャレンジをクリアする必要がありますが、ドメインをAWSのRoute53で管理している場合、dns-route53というプラグインを使うことで、自動で行うことができます。

また、最近はteleportという踏み台用のミドルウェアが存在しており、これを使うとWebUIからターミナルに接続することができます。
サーバーごとにsshのポートを変えたり、いちいち踏み台サーバー越しに接続するのは面倒なので、これも便利です。

== おわりに
本稿では、自宅サーバーを持続可能に運用する一つの解決策として、1マシン1サービスを守ること、
そうしやすくするために自動構築サーバーを作ることと、その作り方の一例の紹介をしました。
また、一緒に構築するとより自宅サーバーを便利に活用できるサーバーも併せて紹介しました。

筆者が今回のように自宅サーバーをリノベーションしてみた個人的な感想としては、
自動構築を考えるのは「カードゲームでオレオレコンボデッキを組んでいるような感じ」で非常に楽しいものでした。
皆様も是非自宅サーバーをより快適にするべく、自動構築を初め、様々なツールを使ってみて、そして共有してくれたらと存じます。

よい鯖缶ライフを！

