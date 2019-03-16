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

Dockerの背景、詳細は今回は説明を割愛します。Dockerの情報は2019年現在、ネット上にたくさん情報がある上
非常に簡単かつ開発がスムーズになるのでぜひ、勉強することをお勧めします。

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
 4. 必要なライブラリのインストール
 5. 本番環境時に動作させたいコマンドを指定

//list[dockerfile_nuxt][フロント側のDockerfile]{
FROM node:10.14-alpine // 1. FROMで今回の環境に合うイメージを設定

RUN mkdir /app // 2. ディレクトリを作成
WORKDIR /app

COPY ./package.json ./yarn.lock ./ // 3. 環境に必要なライブラリを指定しているファイルのコピー

RUN yarn install // 4. 必要なライブラリのインストール
COPY . .

RUN yarn run build 

CMD ["yarn", "run", "start"] // 5. 本番環境時に動作させたいコマンドを指定
//}

//list[dockerfile_python][バック側のDockerfile]{
FROM python:3.6-alpine // 1. FROMで今回の環境に合うイメージを設定

RUN apk --no-cache --update-cache add gcc gfortran (略) && \
    mkdir /app && \
    pip install pipenv 
    // 2. ディレクトリを作成 & 環境に必要なライブラリを指定しているファイルのコピー

WORKDIR /app

COPY ./Pipfile ./Pipfile.lock ./

RUN pipenv install --system  // 4. 必要なライブラリのインストール

COPY . .


ENTRYPOINT ["pipenv", "run"]
CMD ["prod"] // 5. 本番環境時に動作させたいコマンドを指定
//}

MySQLのDockerは今回は、既存のイメージをそのまま利用するので、Dockerfileは作成しません。

=== 2. docker-compose.ymlの作成
次に、1.で作成したdockerファイルを立ち上げて連携できるようにdocker-composeファイルを作成します。
docker-composeとは、開発時などのローカルで複数のDockerコンテナを一つのコマンドで立ち上げられるようにするものです。
docker-composeファイルはプロジェクトの直下に作りましょう。

//list[dockercompose][docker-composeの内容]{
version: '3'
services:
  app: # Nuxt.jsを動かすコンテナの動作定義
    build: ./client/ # Dockerfileの場所を指定
    volumes: # 開発中はローカルにあるファイルを利用して動かしたいので自分のローカルをコンテナにマウントする
      - ./client/:/app
    ports: # ポートの指定
      - "3000:3000"
    command: ash -c "yarn install && yarn run dev" # 本番環境と違うコマンドで開発を動かしたい場合はcommandを使う
    environment: # 環境変数の設定
      API_URL: "http://api:5432"
  api: # Pythonを動かすコンテナの動作定義
    build: ./server/.
    volumes:
      - ./server/:/app
    ports:
      - "5432:5432"
  db: # データベースのコンテナの動作定義
    image: mysql:5.7
    volumes:
      - db_volume:/var/lib/mysql
    ports:
      - "3306:3306"
    environment:
      MYSQL_ROOT_PASSWORD: 'P@ssw0rd'
      TZ: "Asia/Tokyo"
  db_volume: // MySQLのデータ永続化するためにdb_volumeという名前のボリュームコンテナを作成する
    image: busybox:1.29.3
    volumes:
      - db_volume:/var/lib/mysql
    container_name: db_volume
volumes:
  db_volume:
//}

=== 3. 立ち上げ動作確認
ここまででローカルのDocker関連のファイルの作成が完了しました！
早速、動作確認をしてみましょう。
コマンドラインから以下のコマンドを打ち込んで、動作を確認します。

//cmd{
cd NullSuck-AI // docker-compose.ymlが置かれている場所に移動
docker-compose up // docker-composeで起動立ち上げ
//}

ダーっといろんな内容が表示がされると思いますが、アプリとAPIでそれぞれ以下が表示されればOKです。

//list[docker_api_result][APIの表示内容]{
api_1        | INFO: Shutting down
api_1        | INFO: Waiting for application shutdown.
api_1        | INFO: Finished server process [1]
api_1        | INFO: Started server process [1]
api_1        | INFO: Waiting for application startup.
api_1        | INFO: Uvicorn running on http://0.0.0.0:5432 (Press CTRL+C to quit)
//}

//list[docker_app_result][アプリの表示内容]{
app_1        | 10:24:23 Waiting for file changes
app_1        | 10:24:23 Listening on: http://172.19.0.2:3000
//}

実際に、@<code>{http://localhost:3000} や @<code>{http://localhost:5432} にアクセスしてみて
画面が表示されたらOKです。

== 本番環境をKubernetes・GKEで作成しよう
これでローカルにてDockerで動くようになりました。次は実際に本番環境にデプロイして
作成したアプリをみんなで見れるようにしましょう。

今回は、GCPにデプロイします。GCPを使う理由は、付録にてGoogle Data Portalを用いてデータ可視化を行いたいたく、親和性が高いからです。

//table[gcp_kousei][GCPの構成]{
名称  説明
-----------------------
アプリ/APIのデプロイ	Google Kubernetes Engine
Docker Imageの保存	Google Container Registory
DB	Google Cloud SQL
//}

=== 本番環境の構成
本番環境の構成は次の通りです。

//images[GKE_kousei][GKEの構成]

IngressやPod、ServiceなどいろいろKubernetesの専門用語が並んでいますが、ざっくり次のような形で認識しておけばおｋです。
//table[kubernetes][kubenetesの専門用語]{
用語	説明
-----------------------
Ingress	ロードバランサー的な役割を果たす
Service	nginxなどのリバースプロクシ的な役割を果たす
Pod	コンテナのことを指す
//}



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