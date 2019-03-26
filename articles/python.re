={python} PythonでAPIを作成しよう
第3章のデータ分析やデータベースへのアクセスをPythonのAPIとして実装していきましょう。

== 今回、利用する構成
今回、API側では、次の構成を利用します。

//table[kousei][今回の構成]{
名称  説明
-----------------------
APIフレームワーク	responder
O/Rマッパ	SQLAlchemy
機械学習ライブラリ	scikit-learn
//}

今回は、サーバサイドすべてPythonで作成します。
ある一定の規模以上のAI/機械学習アプリを作る場合には
機械学習部のみ今回の形式を用いてPythonでモデルを呼び出すAPIを作成し、その他の部分はGoやJavaなどの静的型付き言語で作成すると開発効率がよいです。

=== APIフレームワーク Responder
PythonのフレームワークというとDjango, Flaskが有名ですが
今回は、responderというフレームワークを利用します。
responderは2018年10月に公開された、新しいフレームワークです。

今回、なぜresponderを扱うのか、理由は次のとおりです。

 * responderがPythonのデファクトスタンダードであるrequestsやpipenvの作者であるKenneth Reitzさんによって作られており、今後flask等に代わりPythonのapiフレームワークのデファクトスタンダードになる可能性が高い
 * flaskのシンプルさとfalconの書きやすさのいいところどりをしている
 * 新しくて私のテンションが上がる

 実際にサンプルアプリを今回作ってみましたが、個人的にはクラスベースでHTTPのハンドラを定義できるのが非常に気持ちよかったです。@<fn>{falcon}

== プロジェクトの構成
//emlist[project_python]{
server/
├── api.py // 起動する
├── db_config.py // DB設定など
├── Dockerfile
├── env
├── handlers.py // ハンドラー
├── __init__.py
├── models.py // モデル
├── nullsuckAi_model.sav // 機械学習のモデル
├── Pipfile 
├── Pipfile.lock
├── services.py
├── settings.py // dotenvの読み込み
└── wine_attributes.csv

1 directory, 12 files
//}

Pythonは、他のJava, C#, Goなどの言語と異なりフォルダごとにパッケージを分けず、一ファイルにクラスをまとめて書くのがスタンダードです。@<fn>{package}

//list[python_dir][Pythonのプロジェクト構成と他言語の比較]{
# JavaやGoの場合
./
├── app.java
├── services
├───── HogeService.java
├── models
├───── Hoge.java
├── handlers
└───── HogeHandler.java

# Pythonの場合
./
├── app.py
├── services.py -> この中にHogeServiceを定義
├── models.py -> この中にモデルを定義
└── handlers.py -> この中にHogeHandlerを定義
//}
=== モジュール管理
本プロジェクトで利用しているresponderやscikit-learnなどのライブラリは　@<tt>{pipenv}というモジュール管理ツールで管理します。
@<tt>{pipenv}は、Rubyでいうとgem、JavaScriptでいうとnpmのようなモジュール管理ツールです。

次のようなPipfileというファイルでモジュール管理や実行するスクリプトを管理できます。

しかも、virtualenvのように仮想環境を作ってくれます。
システムのPythonライブラリのモジュールの整合性が合わなかったりして
アプリが動かない、ということもありません。

何かPythonでアプリなどを作る際は、このpipenvを利用することを強くお勧めします。@<fn>{pipenv}

=== 各モジュールの責務
#@# もしかしたら削ってもいいかも
model、handlerとservicesは次のとおりの依存関係になっています。
//image[service_responsibity][各モジュールの債務]

== 簡単なAPIを作ってreponderで動かしてみよう
本格的な実装に入る前に、まずは簡単なAPIを作って動かすことで
responderとpipenvの使い方に慣れていきましょう。

=== pipenvのインストール
まずはプロジェクトを作成するためにpipenvをインストールしましょう。
Python自体は3.6以上が導入されている前提で進めます。

Pythonが入っていれば、pipenvをインストールするのは簡単です。

//cmd{
    pip install pipenv
//}

これでpipenvが入ります。

=== pipenvでプロジェクトを作ろう
pipenvでプロジェクトを作るのも極めて簡単です。

//cmd{
# プロジェクトの作成
$ mkdir responder_test && cd responder_test
$ pipenv install

Creating a virtualenv for this project…
Pipfile: C:\dev\responder_test\Pipfile
Using c:\python36\python.exe (3.6.3) to create virtualenv…
[    ] Creating virtual environment...Already using interpreter c:\python36\python.exe
(...省略)
Locking [packages] dependencies…
Updated Pipfile.lock (ca72e7)!
Installing dependencies from Pipfile.lock (ca72e7)…
  ================================ 0/0 - 00:00:00
To activate this project's virtualenv, run pipenv shell.
Alternatively, run a command inside the virtualenv with pipenv run.
//}

これでPipfileとPipfile.lockができあがります。
このファイルはJavaScriptでいうところのpackage.jsonとpackage-lock.jsonにあたります。

ここにresponderを追加してみましょう。

//cmd{
$ pipenv install responder

Installing responder…
(略)
Installing dependencies from Pipfile.lock (e3aa62)…
  ================================ 36/36 - 00:00:12
To activate this project's virtualenv, run pipenv shell.
Alternatively, run a command inside the virtualenv with pipenv run.
//}

これでresponderが追加されます。合わせてPipfileとPipfile.lockも更新されています。

=== 簡単なAPIを作ってみよう
GETリクエストすると、キーとバリューがhelloを書かれたjsonを返すエンドポイントを作ります。
次のとおりapi.pyをプロジェクトに作ってみましょう。

//listnum[first_api][api.py][python]{
import responder
api = responder.API()

@api.route("/hello")
def hello(req, resp):
    resp.media = {"hello": "hello"}

if __name__ == '__main__':
    api.run()
//}

@<code>{@api.route("/hello")}のデコレータでルーティングの定義をします。
そして、関数の中で、@<code>{resp.media}にオブジェクトを入れることで、JSONを返すことができます。

flaskと違って、関数の中でreturnを書かなくてよいというのが特徴です。（falcon譲り）

このapp.pyを動かすには、Pipenvで作られた仮想環境で動かす必要があります。
Pipenv作られた仮想環境で動かすためには、次の２つのやり方があります。

 1. @<code>{pipenv shell}で仮想環境のコンソールを立ち上げて@<code>{python} コマンドをたたく 
 2. @<code>{Pipfile} のscriptsにコマンドを書いて @<code>{pipenv run [コマンド定義名]}で動かす

今回は1.の方法で動かしてみましょう。

//cmd{//
$ pipenv shell
Launching subshell in virtual environment…

$ python api.py
INFO: Started server process [21140]
INFO: Waiting for application startup.
INFO: Uvicorn running on http://127.0.0.1:5042 （Press CTRL+C to quit）
//}

実際にcurlコマンドを叩いて確認してみましょう

//cmd{
$ curl localhost:5042/hello                         
{"hello": "hello"}⏎
//}

無事にHelloと帰ってきてたら無事に動いています！


== ハンドラを実装しよう
では、プロジェクトに戻ってハンドラを実装してみましょう。
今回作るAPIは極めてシンプルにエンドポイントは２つだけにします。

//image[handler][エンドポイント設計]

このルーティングを実際に作っていきましょう。

=== ルーティングを定義しよう
まずは、ルーティングを定義します。
ルーティングは@<code>{api.py}につぎのとおり定義します。
//list[routing_python][ルーティング][python]{
from handlers import WineAttributeResource, PredictionResource

models.main()
api = responder.API(
    cors=True,
    allowed_hosts=["*"],
)

api.add_route('/api/wine_attributes', WineAttributeResource)
api.add_route('/api/predict', PredictionResource)
//}

@<code>{api.add_route}でルートを定義できます。　第二引数には、関数かクラスを定義して
該当のルーティングに対応する処理を指定できます。

=== ハンドラを実装しよう
今回は、クラスベースでハンドラを実装してみます。
ハンドラは次のような形で実装できます。

//list[handler_python][ハンドラの実装][python]{
class WineAttributeResource:
    def on_get(self, req, resp):
        wine_attributes = WineAttributeService.get_all()
        resp.media = {
            "status": True,
            "wine_attributes": wine_attributes
        }

class PredictionResource:
    async def on_post(self, req, resp):
        data = await req.media()
        res = PredictionService.predict(data)
        resp.media = {
            "status": True,
            "result": res
        }
//}

クラスに@<code>{on_get}や@<code>{on_post}のメソッドを定義することで
それぞれのエンドポイントのGETリクエストやPOSTリクエストについての挙動を記載できます。

やっていることはシンプルで、リクエストのデータをもとにビジネスロジックであるServiceにデータを投げたりしています。

== 機械学習のモデルを組み込もう
それではいよいよ機械学習のモデルをAPIに組み込んでみましょう。
といってもやることはシンプルでうｓ。
前章で@hidefkn氏に作ってもらったsavファイルをAPIに置いてPOSTデータを食わせます。

=== 機械学習に必要なライブラリをインストールする
まずAPIにも機械学習に必要なscikit-learnやpandas、numpyを導入しましょう。
これもpipenvを利用すれば簡単に導入できます。

//cmd{
$ pipenv install scikit-learn pandas numpy
//}

ちなみにpipenvの唯一の問題なのですが、こういった機械学習系の重めのライブラリを導入すると
@<b>{ゴリゴリに時間がかかります。（半日ほど）}
気長に待つか待てない方は、別の方法で導入しましょう。

=== モデルを読み込むServiceを作成する
では、実際にモデルを組み込んでみましょう。
モデルの組込み計算はServiceにて行います。

//list[service_python][モデルの組込みを行っているservice.py][python]{
from models import WineAttribute, WineAttributeSchema, Prediction
import pickle
from pandas import DataFrame

class PredictionService:
    # pickleで同じフォルダにおいてあるsavファイルを読み込み
    model = pickle.load(open('nullsuckAi_model.sav', mode='rb'))

    @staticmethod
    def predict(data):
        # data(POSTされてきたデータ)を元のモデル作成時と同じpandasのdataframe型に変換する
        df = DataFrame.from_dict(PredictionService.convert_dict(data), orient='columns')
        # 上記で指定したmodelファイルを呼び出し
        predict = PredictionService.model.predict([df['value']])
        # numpyの型で帰ってくるのをfloatに変換
        return float(predict[0])

    # POSTデータの辞書型を変換する
    @staticmethod
    def convert_dict(data):
        converted = {'id': [], 'value': []}
        for k in data:
            converted['id'].append(k['id'])
            converted['value'].append(k['value'])

        return converted
//}

ここでもやっていることは非常にシンプルです。
主に次のことをやっています。

 * pickleで保存されているモデルを呼び出し
 * POSTデータをpandasのデータフレームの形式に変換してpickleから復元したモデルで予測

#@# == DBモデルを実装しよう
#@# では、DBのモデルをPythonで実装しましょう。
#@# 今回は、次のようなテーブル構成で実装を行います。

//footnote[falcon][クラスベースでハンドラを定義できるのはfalcon譲り。flaskはファンクションベースでしか定義できない。]
//footnote[package][実際にJavaみたいに分けてみましたが、いろいろたいへんでした。]
//footnote[pipenv][しかも、pipenvとresponderは同じ作者です。すごすぎ。]