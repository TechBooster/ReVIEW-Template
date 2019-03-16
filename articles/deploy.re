= AIアプリをデプロイしよう
では、今まで作ったアプリを本番にデプロイしてみましょう。

== 今回のデプロイ環境の構成
今回は、次のような構成で動作環境を構築していきます。
開発者がプッシュしただけで、本番環境までデプロイされるCD環境を作ることを目標にします。
//image[figure1][今回の動作環境]

それぞれの要素について説明していきます。

=== GitHub
世界最大のソースコード管理サービスであり
かつもっとも使いやすいサービスです。同様のサービスとしてBitbucketやGitlabがありますが今回はGitHubを用います。

=== CircleCI
継続的デプロイ/デリバリを行うサービスとして、GitHubと組み合わせて使われることが多いサービス。
GitHubとの連携が非常に簡単ですので、今回はこちらを利用します。

=== Google Kubernetes Engine（GKE）
実際に動かす環境として、Google Cloud Platformのサービスのひとつである「Google Kubernetes Engine」を用います。
Kubernetesと名前がついていますが、今回はサクッと動かしたいので、Kubernetesについてはあまりつっこみません。

Dockerでサクッと動かすことに注力して説明していきます。

== ローカル環境をDocker化しよう
実際にデプロイを行う前に、まずはローカル環境でDockerで動かせるようにしましょう。
今回は、説明の都合上、アプリ開発後に、Docker化してますが、実際は、開発の最初にDockerで動かせるようにしておくと
他の人と一緒に開発する際に、環境差異などが出なくてお勧めです。

今回は次の構成でDockerを構成します。
//image[docker][今回のDockerの構成]
appには@<chapref>{nuxt}で作成したNuxt.jsのモジュールを、apiには@<chapref>{python}を配置しています。
DBにはMySQLを利用しています。

Docker化するには、次の手順をそれぞれフロント（Nuxt.js側）とバック（Python API）で行います。

 1. Dockerfileの作成
 2. docker-compose.ymlの作成
 3. 立ち上げ・動作確認

=== 1. Dockerfileの作成
Dockerfileとは環境の構築手順をコードにして、毎回、同じ手順を踏まなくて済むようにするものです。
こちらをフロントとバックそれぞれで作成していきます。

基本的には、両方共同じような動作となっています。

 1. FROMで今回の環境に合うイメージを設定
 2. ディレクトリを作成（気にしない人は作成する必要なし）
 3. 環境に必要なライブラリを指定しているファイルのコピー
 4. 必要な環境の

//list[dockerfile_nuxt][フロント側のDockerfile]{
FROM node:10.14-alpine // 1. 今回の環境のイメージとしてNode.jsを指定

RUN mkdir /app // 2. ディレクトリを作成
WORKDIR /app

COPY ./package.json ./ // 3. 環境に必要なライブラリを指定しているファイルをコピー
COPY ./yarn.lock ./

COPY . .

RUN yarn install && nuxt-ts build
CMD ["yarn", "run", "dev"]
//}

//list[dockerfile_python][バック側のDockerfile]{
FROM python:3.6-alpine // 1.

RUN mkdir /app && pip install pipenv

WORKDIR /app

COPY ./Pipfile ./
COPY ./Pipfile.lock ./

RUN pipenv install

COPY . .


ENTRYPOINT ["pipenv", "run"]
CMD ["production"]
//}

=== 2. docker-compose.ymlの作成
次に、1.で作成したdockerファイルを立ち上げて連携できるようにdocker-compose


=== 3. 立ち上げ動作確認

== 本番環境をKubernetes・GKEで作成しよう

=== 本番環境の構成
=== Kubernetes関連のファイル設定
=== GKE周りの設定
=== ドメイン周りの設定

== CircleCIで自動でデプロイされるようにしよう
=== CircleCIの設定


== GKEの設定
== GKEのCluster作成
//cmd{
gcloud container clusters create nullsuck --num-nodes 2 --zone asia-northeast1-a
//}

//cmd{
gcloud container clusters describe nullsuck --zone asia-northeast1-a
//}

https://medium.com/eureka-engineering/gke-circleci-2-0%E3%81%A7%E7%B6%99%E7%B6%9A%E7%9A%84%E3%83%87%E3%83%97%E3%83%AD%E3%82%A4%E5%8F%AF%E8%83%BD%E3%81%AA%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3%E3%82%92%E3%82%B7%E3%83%A5%E3%83%83%E3%81%A8%E4%BD%9C%E3%82%8B-fc42bef34761
https://medium.com/@gorlemkun/gke%E3%81%ABhello%E3%81%A8%E3%81%A0%E3%81%91%E8%A8%80%E3%82%8F%E3%81%9B%E3%81%9F%E3%81%84-google-kubernetes-engine%E3%81%AB%E7%84%A1%E3%82%92%E3%83%87%E3%83%97%E3%83%AD%E3%82%A4%E3%81%99%E3%82%8B-7c8feea1921c

kubectl create secret generic cloudsql-instance-credentials --from-file=credentials.json=./nullsuck-02cac55f0d96.json
kubectl create secret generic cloudsql-db-password --from-literal=password=fFKMx6I0pBwn9Gz1
kubectl create secret generic cloudsql-db-username --from-literal=username=root
gcloud beta compute ssl-certificates create nullsuck-cert --domains ai.pco2699.net