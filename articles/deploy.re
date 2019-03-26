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

GKEを使う理由は、付録にてGoogle Data Portalを用いてデータ可視化をしたく、その親和性が高いからです。

== ローカル環境をDocker化しよう
実際にデプロイを行う前に、まずはローカル環境でDockerで動かせるようにしましょう。
今回は、説明の都合上、アプリ開発後に、Docker化してますが、実際は、開発の最初にDockerで動かせるようにしておくと
他の方と一緒に開発する際に、環境差異などが出なくてお勧めです。

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
docker-composeとは、開発時などのローカルで複数のDockerコンテナをひとつのコマンドで立ち上げられるようにするものです。
docker-composeファイルはプロジェクトの直下に作りましょう。

//list[dockercompose][docker-composeの内容][yml]{
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
ここまでローカルのDocker関連のファイルの作成が完了しました！
さっそく、動作確認をしてみましょう。
コマンドラインから次のコマンドを打ち込んで、動作を確認します。

//cmd{
$ cd NullSuck-AI // docker-compose.ymlが置かれている場所に移動
$ docker-compose up // docker-composeで起動立ち上げ
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

=={docker_gcr} DockerイメージをGoogle Container Registryに登録してみよう
作ったDockerイメージを後で説明するGKEから利用できるように、DockerイメージをGoogle Container Registryに登録してみましょう。
本作業以降から、Google Cloud SDKを用いて作業を行いますので、@<href>{https://cloud.google.com/sdk/} を参照いただき、コマンドラインからGoogle Cloud SDKを利用できるようにしておいてください。

=== GCPのプロジェクト作成
まず、GCPを使えるようにするには、Googleアカウントにログインしたうえで、GCP上でプロジェクトを作成する必要があります。
自分のGoogleアカウントでログインし、@<href>{https://console.cloud.google.com/} にアクセスしましょう。

次に示すとおり、「プロジェクトの作成」からプロジェクトを作成していきます。

//image[gcp_making_project][GCPのプロジェクト作成 手順1]

//image[gcp_making_project2][GCPのプロジェクト作成 手順2]

//image[gcp_making_project3][GCPのプロジェクト作成 手順3]

今回の例では、プロジェクト名は「nullsuck」、請求先アカウントは登録しています。@<fn>{gcp_account}

=== Dockerイメージをpush
プロジェクトができたので、これでGoogle Container RegistryにDockerイメージをpushできます。
実際に、pushを行ってみましょう。

自分のローカル環境でDockerのイメージをbuildするところから始めます。

//cmd{
$ cd NullSuck-AI // プロジェクト直下に移動
$ docker build -t nullsuck_ai_app ./client/. // アプリのイメージビルド
//}

@<code>{docker build -t 名前}で指定した名前でイメージをビルドできます。

次のような形で @<tt>{Successfully built} と出力されればOKです。
同様にAPI側もイメージビルドしましょう。

//cmd{
Done in 55.48s.
Removing intermediate container afb9b8e3b712
 ---> cdf467249627
Step 8/8 : CMD ["yarn", "run", "start"]
 ---> Running in fea349a64cbb
Removing intermediate container fea349a64cbb
 ---> 30d33b376c34
Successfully built 30d33b376c34
Successfully tagged nullsuck_ai_app:latest
//}

今度はこれを、Google Container Registryに命名ルールに沿って別名をつけてあげましょう。
Google Container Registryでは次の命名ルールに沿う必要があります。
//emlist{
  {asia|us|eu}.gcr.io/プロジェクトID/コンテナイメージの名前
//}

頭の、asiaなどの地域を指定することによって、イメージがどの場所でホストされるか指定できます。
今回は asia でやってみましょう。

//cmd{
$ docker tag nullsuck_ai_app asia.gcr.io/nullsuck/app
//}

このコマンドで@<tt>{nullsuck_ai_app}に別名として@<tt>{asia.gcr.io/nullsuck/app}とつけることができました。
Google Cloud SDKを用いてGoogle Container Registryにpushしてみましょう。

//cmd{
$ gcloud docker -- push asia.gcr.io/nullsuck/app
//}

こんな感じで、表示が出れば、OKです。

//emlist{
  The push refers to a repository [asia.gcr.io/nullsuck/app]
//}

実際にGCP上のページでも「ナビゲーションメニュー -> ツール - Container Registry」を開くとイメージが確認できます。

//image[container_registry][GCP上のコンテナイメージ]

これで、イメージをGoogle Container Registryに登録できました。
登録したイメージは次の章で実施に動かしていきます。


== 本番環境をKubernetes・GKEで作成しよう
これでローカルにてDockerで動くようになりました。次は実際に本番環境にデプロイして
作成したアプリをみんなで見れるようにしましょう。

//table[gcp_kousei][GCPの構成]{
名称	説明
-----------------------
アプリ/APIのデプロイ	Google Kubernetes Engine
Docker Imageの保存	Google Container Registory
DB	Google Cloud SQL
//}

=== 本番環境の構成
本番環境の構成は次のとおりです。

//image[GKE_kousei][GKEの構成]

IngressやPod、ServiceなどいろいろKubernetesの専門用語が並んでいますが、ざっくり次のような形で認識しておけばおｋです。
//table[kubernetes][Kubernetesの専門用語]{
用語	説明
-----------------------
Ingress	ロードバランサ的な役割を果たす
Service	nginxなどのリバースプロキシ的な役割を果たす
Pod	コンテナのことを指す
//}

==={kubernetes} Kubernetes関連のファイル設定
では、実際に、構成を元に、Kubernetes関連のファイルを作成していきましょう。
Kubernetesは、@<code>{kubectl}というコマンドラインツールで、設定を行うことも可能です。
しかし、同じ環境を作るのに、毎回コマンドをたたくのだと不便です。
そのため、ymlファイルに保存しておき、適用する、というのがよく行われる手段です。

今回は次のような構成でファイルを作成していきます。

//list[kubernetes_files][今回作るKubernetesのファイル構成]{
k8s // k8sというフォルダにまとめて管理する
├── api // api関連のKubernetesファイル
│   ├── api-deployment.yml
│   └── api-service.yml
└── app // app関連のKuberntesファイル
    ├── app-deployment.yml
    ├── app-ingress.yml
    └── app-service.yml
//}

それでは次から各ファイルの内容について説明していきます。
まずはdeploymentからです。

deploymentは、Podの情報を含め、Podをどのくらい複製して作るかや配置方法などを決めるファイルです。
Podを作る際には、Pod毎にファイルを作ることも可能ですが、今回のような小さいアプリであればdeploymentファイルを作れば十分でしょう。

//list[deployment_app][app-deployment.yml][yml]{
apiVersion: apps/v1
kind: Deployment # deploymentを示す
metadata:
  name: app
spec:
  # ここで何個 Podを複製するか決める。
  # 今回はコストを抑えたいので1で。
  replicas: 1 
  selector:
    matchLabels: # 後のcontainersのnameと合わせる
      app: app
  template:
    metadata: # 後のcontainersのnameと合わせる
      labels:
        app: app
    spec:
      # コンテナの情報を記載する。docker-composeで書いた内容とほぼ同じでOK
      containers: 
      - name: app
        # GCRでpushした内容を指定
        image: asia.gcr.io/nullsuck/app:latest 
        ports:
        - containerPort: 3000
        env:
          - name: API_URL
            value: "http://api:5432"
//}

同様にapiもdeploymentファイルを作ります。

//list[deployment_api][api-deployment.yml][yml]{
// 最初はappと同じなので、省略
      containers:
      - name: api
        image: asia.gcr.io/nullsuck/api:latest
        ports:
        - containerPort: 5432
        env:
          - name: PY_ENV
            value: prod
          - name: DB_HOST # DBホストはProxyに向ける
            value: 127.0.0.1:3306
          - name: DB_USER # ユーザ名・パスワードはGKEのSecretsから設定
            valueFrom:
              secretKeyRef:
                name: cloudsql-db-username
                key: username
          - name: DB_PASSWORD
            valueFrom:
              secretKeyRef:
                name: cloudsql-db-password
                key: password
      # APIからCloud SQLにアクセスできるように専用のProxyをPodとして用意
      - name: cloudsql-proxy 
        image: gcr.io/cloudsql-docker/gce-proxy:1.11
        command: ["/cloud_sql_proxy",
                  "-instances=nullsuck:asia-northeast1:nullsuck-db=tcp:3306",
                  "-credential_file=/secrets/cloudsql/credentials.json"]
        securityContext:
          runAsUser: 2
          allowPrivilegeEscalation: false
        volumeMounts:
          - name: cloudsql-instance-credentials
            mountPath: /secrets/cloudsql
            readOnly: true
          - name: ssl-certs
            mountPath: /etc/ssl/certs
          - name: cloudsql
            mountPath: /cloudsql
      volumes: # Secretを利用するためにボリュームを設定
        - name: cloudsql-instance-credentials
          secret:
            secretName: cloudsql-instance-credentials
        - name: ssl-certs
          hostPath:
            path: /etc/ssl/certs
        - name: cloudsql
          emptyDir:
//}

appファイルと異なる点は、Podが２つあり、ひとつはapi用、ひとつはCloud SQLへアクセスするプロキシ用を用意しています。
また、Cloud SQLにアクセスするために必要なCredentialsやユーザー名・パスワード名は、すべてGKEのSecretを通して
取得するようにしています。

GKEのSecretはのちほど、設定します。


次に、serviceファイルを作ります。
//list[service_app][app-service.yml][yml]{
apiVersion: v1
kind: Service # Serviceを指定
metadata:
  name: app-svc
spec:
  type: NodePort 
  selector:
    app: app # deploymentのappと内容を合わせる
  ports: # 公開するポートに合わせる
    - protocol: TCP
      port: 3000
      targetPort: 3000
      name: http
//}

ポイントとしては、@<tt>{spec.selector}の属性名をdeploymentで設定した@<tt>{app}と同じ名前にすることです。
これも同様にapiも作成してください。

最後に、ingressを作成します。
//list[ingress_app][app-ingress.yml][yml]{
apiVersion: extensions/v1beta1
kind: Ingress # Inrgressを指定
metadata:
  name: app-ingress
spec:
  rules:
    - http:
        backend:
          # appのServiceを指定
          serviceName: app-svc
          # appのServiceのポートと同じものを指定 
          servicePort: 3000 
//}

これでKubernetes関連のファイルが全て作成できました。

=== GKE周りの設定
実際に、GKEでKubernetesのClusterを作成してみましょう。
Clusterとは、さきほど、定義したIngressやService、Podなどが載る基盤です。

次のコマンドでClusterを作ることができます。
//cmd{
$ gcloud container clusters create nullsuck --num-nodes 2 --zone asia-northeast1-a
//}

@<tt>{num-nodes}は、Clusterの個数を指定できます。
冗長性の観点では2以上にするとよいですが、そのぶんお金がかかるのでとりあえず1でもよいかもしれません。
@<tt>{zone}は、マシンの地域を指定しています。@<fn>{gcloud_zone}

ここから、@<tt>{kubectl}というKubernetes用のcliツールでKubernetesを操作します。

まず、GKEから認証情報を取得してきて、あたかもローカルから@<tt>{kubectl}を用いて
Kubernetesが操作できる環境にします。

//cmd{
$ gcloud container clusters get-credentials nullsuck
//}

これでNullSuckプロジェクトのGKE環境を@<tt>{kubectl}で操作できます。
そして、さきほど、作ったymlファイル類を適用していきます。

//cmd{
// deploymentを適用
$ kubectl apply -f api-deployment.yaml
$ kubectl apply -f app-deployment.yaml

// serviceを適用
$ kubectl apply -f api-service.yaml
$ kubectl apply -f api-service.yaml

// ingressを適用
$ kubectl apply -f app-ingress.yaml
//}

これで次のコマンド叩いてIPアドレスを確認しましょう

//cmd{
$ kubectl get ingress
//}

IPアドレスにアクセスしてみて、問題なくページが表示されていれば
無事にデプロイが完了しています！

==={cloudsql} Cloud SQLの設定・Credientialsの設定
次に、Cloud SQLを作っていきます。こちらもGoogle Cloud SDKで作成していきます。@<fn>{cloud_sql}

次のコマンドでCloud SQLのインスタンスを作成します。

//cmd{
$ gcloud beta sql instances create nullsuck-db
                     --tier=db-f1-micro --activation-policy=ALWAYS
//}
今回は、DBインスタンス名をnullsuck-db インスタンスタイプは一番、小さいdb-f1-microを利用します。

Cloud SQLは、第一世代と第二世代のインスタンスがあります。
GKEからプロキシで接続するためには、第二世代のインスタンスを利用する必要があります。
第二世代は頭にdb- とついているので、それを利用するようにしましょう。

アクセスできるようにするため、rootのパスワードを設定します。

//cmd{
$ gcloud sql instances set-root-password nullsuck-db
                                --password [任意のパスワードを設定]
//}

次に、このCloud SQLをGKEから接続できるようにするためCredientialsを設定しましょう。
まずはCloud SQLからアクセスできるように、コンソール画面からサービスアカウントを設定します。

//image[creating_service]
//image[creating_service2]
//image[creating_service3]
//image[creating_service4]
//image[creating_service5]
//image[creating_service6]

この手順で取得したjsonファイルをGKEのSecretsに設定します。@<fn>{gke_secrets}

次のコマンドでGKEのSecretsにjsonファイルを格納できます。
この格納したファイルはAPIのPodのCloudSQL プロキシが呼び出されます。

//cmd{
$ kubectl create secret generic cloudsql-instance-credentials
  --from-file=credentials.json=[先程ダウンロードしたjsonのパス]
//}

次に、データベースのユーザー名とパスワードをSecretsに設定します。
今回は、rootをユーザー名に指定してますが、別にユーザーを作成して設定することをお勧めします。
//cmd{
$ kubectl create secret generic cloudsql-db-password
                         --from-literal=password=[パスワード名]
$ kubectl create secret generic cloudsql-db-username
                         --from-literal=username=root
//}

これで、Cloud SQLにつなげる準備ができました。


#@# === ドメイン周りの設定
#@# #@# 書けたら書く

== CircleCIで自動でデプロイされるようにしよう
最後に、今までの手順をすべて、CircleCIに設定して
GitHubのmasterブランチが更新されたら、自動的にデプロイが更新されるようにしましょう。

CircleCIとGitHubの連携はCircleCIから行います。（今回は省略）
そしてリポジトリの直下に次のような形でディレクトリ、ymlファイルを作成します。
//list[circleci_dir][CircleCIのディレクトリ]{
NullSuck-AI/.circleci
└── config.yml
//}

=== CircleCIを設定する手順
CircleCIでデプロイするために次の3つの手順を踏みます。

 1. CircleCIでデプロイするためのGCPのサービスアカウントの設定
 2. Kubernetesの設定ymlを一部、書き換える
 3. config.ymlを作成する

==== CircleCIでデプロイするためのサービスアカウント設定
CircleCIでデプロイするためにサービスアカウントを設定します。
基本的な内容は @<hd>{cloudsql} のサービスアカウント作成手順と同じですが
権限を次の様に設定します。

 * Kubernetes > Kubernetes Engine 開発者
 * Storage > Storage 管理者

上記は、GKEおよびGCRを使えるようにするためです。

そして、取得してきたjsonファイルの中身を次のような形で
環境変数としてCircleCIに設定します。

//image[circleci_env][CircleCIの環境変数設定]

==== Kubernetesの設定ymlを一部、書き換える
@<hd>{kubernetes}で設定したymlを、一部書き換えます。
これは、Dockerのイメージを毎コミットごとに管理できるようにするためです。

//emlist{
  asia.gcr.io/nullsuck/api:${CIRCLECI_SHA1}
  ↓
  asia.gcr.io/${PROJECT_NAME}/api:${CIRCLECI_SHA1}
//}

==== config.ymlを作成する
config.ymlを作成していきます。
config.ymlは簡単にいうとpushされた後に動かしてほしいコマンドを書いておくものです。
ここにさきほど、手動で設定したGCRやGKEの設定手順をすべて、まとめて書いていきます。

次が実際のconfig.ymlファイルです。

//list[configyml][config.yml][yml]{
version: 2
jobs:
  deploy_to_prod:
    working_directory: /app
    environment: # プロジェクト名などの環境変数を宣言
      - PROJECT_NAME: "nullsuck"
      - （略）
    docker:
      - image: google/cloud-sdk:237.0.0 # googleのCloudSDKのイメージを利用
    steps:
      - checkout
      - run:
          name: Setup Google Cloud SDK
          command : |
            apt-get install -qq -y gettext # あとで利用するenvsubstコマンドのため導入
            echo $GCLOUD_SERVICE_KEY > ${HOME}/gcloud-service-key.json # Google
            gcloud auth activate-service-account --key-file=${HOME}/gcloud-service-key.json
            gcloud --quiet config set project ${GOOGLE_PROJECT_ID}
            (省略 Google Cloud SDKの設定)
      - setup_remote_docker
      - run:
          name: Docker build and push
          command: |
            docker build \
              -t ${PROJECT_NAME}/app ./client/.
            docker tag ${PROJECT_NAME}/app asia.gcr.io/${PROJECT_NAME}/app:${CIRCLE_SHA1}
            docker build \
              -t ${PROJECT_NAME}/api ./server/.
            (省略 DockerのGCRへのpush作業)
      - run:
          name: Deploy to Kubernetes
          command: |
            bash ./concat.sh
            envsubst < ./k8s.yml > ./patched_k8s.yml
            kubectl apply -f ./patched_k8s.yml

workflows:
(省略)
//}

基本的には、今までやってきた手順をすべて、コマンドとして起こしたものです。
本ymlファイルで追加で行っていることとしては次のとおりです。

 * Kubernetesの設定ファイルをすべてひとつのファイルにまとめている
 * 前の手順で設定した環境変数に変換するため @<code>{envsbst} でymlファイルの変換を行っている。

このファイルを実際にコミットして、CircleCIが動くことを確認できれば、デプロイの作業はすべて完了です。
お疲れさまでした。

これでヌルサクAIアプリがネットに公開されました。

===[column] コンテナ開発で困った！あるある

サンプルアプリｄえコンテナを使って開発していて実際に困ったことをまとめてみました。

==== 外からアクセスできない！
ホスト名の指定を@<tt>{localhost}にしていると外からアクセスできません。
@<tt>{0.0.0.0}にするとアクセスできます。

==== ブラウザからAPIにたたこうとするけど、CORSエラーでるし、そもそも公開していない！
Nuxt.jsだとプロキシの機能があるので、それを使いましょう。
これで、ブラウザ -> Nuxt.js -> APIで通信されます。

nuxt.config.tsで次のように設定します。
//emlist{
axios: {
  proxy: true
},
proxy: {
  '/api': process.env.API_URL
},
//}

===[/column]


//footnote[gcp_account][GCPを初回登録される方は、無料枠があるので、請求先アカウントを利用せずに無料で使えるかもしれないです。]
//footnote[gcloud_zone][Google Cloud SDKの初期化時（@<code>{gcloud init}実行時）にDefault Zoneを指定していれば省略できます。]
//footnote[cloud_sql][コンソール画面上からも作成できます。]
//footnote[gke_secrets][Secretとは、パスワードや認証キーなどを暗号化して格納しておくKubernetesのオブジェクトです。詳細は @<href>{https://cloud.google.com/kubernetes-engine/docs/concepts/secret?hl=ja} 参照]