
= Rails + chewy でなんとなくはじめる Elasticsearch

== はじめに
グリー株式会社の新卒2年目エンジニアのネゴロと申します。

現在はグリーの子会社で「おでかけ」に関する様々なサービスを展開しているアウモ株式会社(https://aumo.co.jp/)でサーバーサイドエンジニアをしており、開発言語は主にRuby(Rails)です。

今回は、実務で非常にお世話になっているElasticsearchについてご紹介しようと思います。

Elasticsearchをまだ触ったことがない方や、RailsでElasticsearchの導入を考えている方などのご参考になれば幸いです。

この章のコンセプトとしては、「とりあえず動かしてみる！」です。

理論と実践のバランスは、エンジニアリング面でも非常に重要ですが、自分はまず手を動かして動作するものを作ってみて、細かいところは後から学習するスタンスをよく取っています。

そのため、この記事でもとりあえずRailsの環境でElasticsearchを扱えるようにし、インデックスを作成したり、クエリを投げたりするところまで一気にさらっていきたいと思います。

この章では以下のことをご紹介しています。

 * DockerによるRuby on Rails / MySQL / Elasticsearchの環境構築
 * gem「chewy」を用いたインデックスの作成、クエリ等

また、今回は「とりあえず動かしてみる！」ことが目的であり、以下の情報は参考になる外部リソースが多く存在するため、記述していません。

 * Elasticsearchの具体的なクエリの書き方
 * Elasticsearchの技術自体の詳細

では、はじめていきます！

== Elasticsearchとは

Elasticsearchとは、Luceneベースの全文検索エンジンで、Elastic社が開発しています。大容量のデータに対して高速に検索を行うことができます。
https://www.elastic.co/jp/elasticsearch/

Elasticsearchでは各種データをJSONドキュメントとして、インデックスに登録していきます。
理解の補助のため、以下のようなリレーショナルデータベース(RDB)との対応イメージを持っていただくとわかりやすいかもしれません。

 * インデックス => RDBのテーブル
 * ドキュメント => RDBのレコード

インデックスの作成やドキュメントの登録などの操作に関しては、基本的にはRESTful インターフェースを用います。

例えば、`library`というインデックスにドキュメントを登録する場合

//cmd{
PUT /library/_doc/1
{
  "title": "Norwegian Wood",
  "name": {
    "first": "Haruki",
    "last": "Murakami"
  },
  "publish_date": "1987-09-04T00:00:00+0900",
  "price": 19.95
}
//}

のようにリクエストします。

またElasticsearchはスキーマレスなので、スキーマを定義しなくても、リクエストされたドキュメントから自動でスキーマを設定してくれます。

例えば、

//cmd{
{
  "one_int": 1,
  "one_str": "1"
}
//}

というドキュメントがリクエストされた場合、`one_int`は数値型、`num_str`は文字列型と推測され、スキーマが設定されます。

もちろん、インデックス作成のタイミングで事前にスキーマを設定することも可能で、その場合マッピングを定義します。
マッピングとはドキュメントのスキーマ定義で、フィールド名や型などを保持するメタ情報です。

マッピングで指定できる型の一覧はこちらをご参照ください。

https://www.elastic.co/guide/en/elasticsearch/reference/current/mapping-types.html

マッピングはインデックス作成のタイミングで、以下のように定義できます。

//cmd{
PUT /blogs
{
  "mappings": {
    "properties": {
      "content" : {
        "type": "text",
      },
      "title" : {
        "type": "keyword"
      },
      "published_at": {
        "type": "date"
      }
    }
  }
}
//}

Elasticsearchに関する概要は以上です。

より詳細を知りたい方は、Elastic社の公式チュートリアルがわかりやすいのでおすすめです。

https://www.elastic.co/guide/jp/elasticsearch/reference/current/getting-started.html

Elasticsearchはドキュメントも充実しているので助かりますね。

== Elasticsearchの活用ケース

「大量のレコードが格納されたRDBのテーブルに対して、集計クエリをリアルタイムでいくつも投げなきゃいけないが、レスポンスが遅くて困っている。。」

「BigQueryでデータ分析のためにいろんなクエリを試したいけど、料金が怖い。。」

もし上記のようなケースに遭遇したら、Elasticsesarchはとても心強い味方になるかもしれません。

実際にアウモでも大容量のデータを対象とした検索・集計においてElasticsearchを活用しています。

 * アプリでのユーザー・スポット(...アウモにおける飲食店やレジャー施設などのお出かけスポットの総称)検索
 * ユーザーバッジ機能(詳細: https://techblog.aumo.co.jp/articles/667)

ほかにも、日々大量のデータが蓄積されるログ情報の分析など、Elasticsearchは幅広い用途で活躍します。

== Rails + chewy で Elasticsearch を触ってみよう

ここから本題です。
DockerでRuby on Railsのサンプルアプリケーション/MySQL/Elasticsearchの環境構築を行い、MySQLに格納されているデータを元にElasticsearchのインデックス作成・ドキュメント登録、クエリの実行まで一気に進めていきます。

まず、この章のために用意した以下のGitHubリポジトリをローカル環境でcloneしてください。

https://github.com/watarun54/rails-chewy

cloneが終わったら以下のコマンドを実行してください。

//cmd{
cd ./rails-chewy
docker-compose up -d # ...(1)
docker-compose exec api rails db:seed # ...(2)
docker-compose exec api bundle exec rake chewy:reset # ...(3)
//}

それぞれのコマンドについて解説します。

まず(1)では、RailsとElasticsearch、MySQLの各コンテナを立ち上げています。

`docker-compose.yml`ファイルの内容は以下の通りです。

//listnum[Docker Compose][./docker-compose.yml][yml]{
version: '3'
services:
  api:
    restart: always
    build:
      context: .
    volumes:
      - .:/api
      - /api/node_modules
      - public-data:/api/public
      - tmp-data:/api/tmp
      - log-data:/api/log
      - bundle-data:/usr/local/bundle
    command: sh launch.sh
    tty: true
    stdin_open: true
    depends_on:
      - db
      - elasticsearch
    ports:
      - 3000:3000

  db:
    restart: always
    image: mysql:8.0.25
    platform: linux/amd64
    environment:
      MYSQL_DATABASE: test_development
      MYSQL_USER: admin
      MYSQL_ROOT_PASSWORD: password
      MYSQL_PASSWORD: password
      TZ: 'Asia/Tokyo'
      MYSQL_ROOT_HOST: "%"
    tty: true
    command: mysqld --character-set-server=utf8mb4 --explicit_defaults_for_timestamp=true --default-authentication-plugin=mysql_native_password    
    ports:
      - 3306:3306

  elasticsearch:
    image: docker.elastic.co/elasticsearch/elasticsearch:7.2.0
    environment:
      discovery.type: single-node
      cluster.name: docker-cluster
      bootstrap.memory_lock: 'true'
      ES_JAVA_OPTS: -Xms512m -Xmx512m
    ulimits:
      memlock:
        soft: -1
        hard: -1
    ports:
      - 9200:9200
    depends_on:
      - db

volumes:
  public-data:
  tmp-data:
  log-data:
  bundle-data:
//}

`api`がRails、`db`がMySQL、`elasticsearch`がElasticsearchのコンテナです。

dbとelasticsearchコンテナに関しては、公式Dockerイメージを指定しています。
dbはもちろんelasticsearchのコンテナにも様々なパラメータを指定でき、例えば上記では`ES_JAVA_OPTS`という環境変数でJava VMのヒープサイズ上限を指定しています。
メモリが少なくて立ち上がらない場合は、コンテナ自体のメモリ上限の引き上げだけでなく、こちらのヒープサイズ上限の引き上げも検討してみてください。

続いて、(2)では今回利用するシードデータの投入を行っています。

今回シードデータとして利用するモデル(テーブル)は`User`と`Receipt`の２つで、それぞれユーザーとレシートを表しており、User:Receiptは1:多の関係です。(1ユーザーは複数のレシートを保持している)

`rails db:seed`

はRails標準装備のコマンドで、実行すると`db/seeds.rb`ファイルのコードを元にデータ投入を行います。
このファイルの内容は以下の通りです。

//listnum[Seed][db/seeds.rb ][ruby]{
users = []
500.times do |each_idx|
  users << User.new(
              name: "user_#{each_idx}",
              age: rand(20..65)
            )
end
User.import! users

user_ids = User.all.pluck(:id)

100.times do |batch_idx|
  receipts = []
  1000.times do |each_idx|
    receipts << Receipt.new(
                  title:  "receipt_#{batch_idx}_#{each_idx}",
                  amount: rand(1000..100_000),
                  issued_at: Random.rand(Date.new(2018, 1, 1)..Date.new(2022, 1, 1)),
                  user_id: user_ids.sample
                )
  end
  Receipt.import! receipts
end
//}

ここでは`User`モデルのレコード500件、`Receipt`モデルのレコード100,000件を生成しています。

RailsではActiveRecordというORマッパーが標準で提供されており、

//cmd{
User.find(1)
//}

のように`<モデル名>.<メソッド>`の形式でRDBなどの外部ストレージに対して様々なリクエストを実行できます。

ActiveRecordでは、スキーマファイルは`db/migrate/`ディレクトリ以下に、モデルファイルは`app/models/`ディレクトリに配置するお約束になっています。

`rails db:migrate`

というコマンドを実行することで、`db/migrate/`以下のスキーマ情報をDBに反映します。(マイグレーション)

`User`と`Receipt`のスキーマは以下の通りです。

//listnum[User schema][db/migrate/20220829050533_create_users.rb ][ruby]{
class CreateUsers < ActiveRecord::Migration[7.0]
  def change
    # ユーザー
    create_table :users do |t|
      t.string :name # 名前
      t.integer :age # 年齢

      t.timestamps
    end
  end
end
//}

//listnum[Receipt schema][db/migrate/20220829055309_create_receipts.rb ][ruby]{
class CreateReceipts < ActiveRecord::Migration[7.0]
  def change
    # レシート
    create_table :receipts do |t|
      t.string :title # 名前
      t.integer :amount # 金額
      t.datetime :issued_at # 発行日
      t.references :user, null: false, foreign_key: true # 発行したユーザー

      t.timestamps
    end
  end
end
//}

最後に(3)ではElasticsearchのインデックスの作成、ドキュメントの登録を行なっています。

今回は`chewy`というActiveRecordのモデルの内容をもとに簡単にElasticsearchを扱える便利なgemを利用します。

https://github.com/toptal/chewy

chewyは様々なrakeコマンドを提供しており、(3)で実行している以下のコマンドはインデックスの再作成とドキュメント登録を実行するコマンドです。

//cmd{
bundle exec rake chewy:reset
//}

このコマンドを実行して以下のような内容が出力されたら、インデックス作成・ドキュメント登録は完了です。

//cmd{
############################################################
WARN: You are risking to lose some changes during the reset.
      Please consider enabling journaling.
      See https://github.com/toptal/chewy#journaling
############################################################
Resetting UsersIndex
  Imported UsersIndex in 12s, stats: index 500
  Imported Chewy::Stash::Specification in 1s, stats: index 1
Total: 11s
//}

この出力の内容から`UsersIndex`というインデックスがリセットされていることがわかります。

では、`chewy`は一体どのファイルを参照してこのインデックスを扱っているのかというと、`app/chewy/`ディレクトリ以下に配置したインデックスのファイルを参照しています。

今回は`User`モデルに対するインデックスを作成しました。内容は以下の通りです。

//listnum[UsersIndex][app/chewy/users_index.rb ][ruby]{
class UsersIndex < Chewy::Index
  index_scope User.includes(:receipts)
  field :name, type: 'keyword'
  field :age, type: 'integer'
  field :receipts do
    field :title, type: 'keyword'
    field :amount, type: 'integer'
    field :issued_at, type: 'date'
  end

  class << self
    # 年齢が20歳以上25歳以下の月別平均金額を出す
    def sample_aggs
      UsersIndex
        .query(range: { age: { gte: 20, lte: 25 } })
        .aggs(
          by_month: {
            date_histogram: { 
              field:    "receipts.issued_at",
              interval: "month",
              format:   "yyyy-MM"
            },
            aggs: {
              avg_amount: {
                avg: { field: "receipts.amount"}
              }
            }
          }
        ).aggs
      end
  end
end
//}

上記のコードにはマッピング定義とクエリが含まれます。

まずマッピング定義は以下の箇所です。

//listnum[UsersIndex Mapping][app/chewy/users_index.rb ][ruby]{
class UsersIndex < Chewy::Index
  index_scope User.includes(:receipts)
  field :name, type: 'keyword'
  field :age, type: 'integer'
  field :receipts do
    field :title, type: 'keyword'
    field :amount, type: 'integer'
    field :issued_at, type: 'date'
  end
  ...
end
//}

`index_scope`ではインデックスの対象となるモデルのスコープを定義します。chewyはここで定義したscopeをドキュメント登録の際に参照しています。
`field`ではそれぞれドキュメントのフィールド名と型を指定します。

ここでreceiptsフィールドは、以下のようにユーザーに紐づくレシートを取得するActiveRecordのクエリに相当するものです。

//cmd{
User.find(xxx).receipts
//}

chewyのfieldではその名前に合わせて自動的にActiverecordの関連モデルを引っ張ってきてくれます。

index_scopeで`includes(:receipts)`を記述している理由は、インデックス作成・ドキュメント登録で、各ユーザーに対してreceiptsを呼び出す際に、N+1が発生することを防ぐためです。

このように、chewyを用いるとActiveRecord特性に合わせてElasticsearchのマッピング定義を行うことが可能です。

最後にクエリ部分ですが、今回サンプルとして`年齢が20歳以上25歳以下の月別平均金額`を出力するようなクエリを用意しました。

//listnum[UsersIndex Query][app/chewy/users_index.rb ][ruby]{
class UsersIndex < Chewy::Index
  ...
  class << self
    # 年齢が20歳以上25歳以下の月別平均金額を出す
    def sample_aggs
      UsersIndex
        .query(range: { age: { gte: 20, lte: 25 } })
        .aggs(
          by_month: {
            date_histogram: { 
              field:    "receipts.issued_at",
              interval: "month",
              format:   "yyyy-MM"
            },
            aggs: {
              avg_amount: {
                avg: { field: "receipts.amount"}
              }
            }
          }
        ).aggs
      end
  end
end
//}

chewyでは`HogeIndex.query`や`HogeIndex.aggs`などElasticsearchの操作を扱うRubyのメソッドが様々用意されています。

実際に上記のクエリを実行してみましょう。

//cmd{
docker-compose exec api rails c
//}

でrails consoleに入り

//cmd{
UsersIndex.sample_aggs
//}

を実行すると、以下のような結果が返ってきます。(レスポンス早い！)

//cmd{
{"by_month"=>
  {"buckets"=>
    [{"key_as_string"=>"2018-01", "key"=>1514764800000, "doc_count"=>55, "avg_amount"=>{"value"=>50379.749225441956}},
     {"key_as_string"=>"2018-02", "key"=>1517443200000, "doc_count"=>53, "avg_amount"=>{"value"=>50397.307597574836}},
     {"key_as_string"=>"2018-03", "key"=>1519862400000, "doc_count"=>55, "avg_amount"=>{"value"=>50379.749225441956}},
     {"key_as_string"=>"2018-04", "key"=>1522540800000, "doc_count"=>55, "avg_amount"=>{"value"=>50379.749225441956}},
     {"key_as_string"=>"2018-05", "key"=>1525132800000, "doc_count"=>54, "avg_amount"=>{"value"=>50384.121815655395}},
     ...
    ]
  }
}
//}

`key_as_string`で月毎のデータが取得できていることがわかります。
平均金額は`avg_amount`のvalueの値が相当します。

今回は10万件程度のデータセットを対象としましたが、実務のElasticsearchの運用ではもっと多くのデータ量を扱う場合が多く、データ量が多いほどElasticsearchとRDB等のクエリの速度差は明確になっていきます。（それぞれのホストのスペックや構成に依存するところもあります）

== 最後に

以上で、RailsでElasticsearchを扱うための一通りの流れは経験できました。

もちろんElasticsearchやchewyに関して、ここでは説明しきれなかった事がたくさんあります。

それらは全て公式ドキュメント等にお任せしようと思います笑(丸投げ)

この章を読んで、「Elasticsearch使ってみようかな」と思ってもらえたら嬉しいです。

== (付録) おすすめサービス・文献

 * クラウドでElasticsearchの管理やその他の便利ツールを提供する「Elastic Cloud」
https://www.elastic.co/jp/cloud/

 * Elasticsearchのshared数の決め方
https://www.elastic.co/jp/blog/how-many-shards-should-i-have-in-my-elasticsearch-cluster

 * Google Dataflowを用いて、BigQueryのテーブルのデータを直接Elasticsearchにインジェストする
https://www.elastic.co/jp/blog/ingest-data-directly-from-google-bigquery-into-elastic-using-google-dataflow
