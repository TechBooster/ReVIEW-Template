={python} PythonでAPIを作成しよう
第3章のデータ分析やデータベースへのアクセスをPythonのAPIとして実装していきましょう。

== 今回、利用する構成
今回、API側では、次の構成を利用します。

//table[kousei][今回の構成]{
名称  説明
-----------------------
APIフレームワーク	responder
ORM	SQLAlchemy
機械学習ライブラリ	scikit-learn
//}

今回は、サーバサイドすべてPythonで作成します。
ある一定の規模以上のAI/機械学習アプリを作る場合には
機械学習部のみ今回の形式でPythonでモデルを呼び出すAPIを作成し、その他の部分はGoやJavaなどの静的型付き言語で作成すると開発効率が良いと思います。

=== APIフレームワーク Responder
PythonのフレームワークというとDjango, Flaskが有名ですが
今回は、responderというフレームワークを利用します。
responderは2018年10月に公開された、新しいフレームワークです。

今回、なぜresponderを扱うのか、理由は次のとおりです。

 * responderがPythonのデファクトスタンダードであるrequestsやpipenvの作者であるKenneth Reitzさんによって作られており、今後flask等に代わりPythonのapiフレームワークのデファクトスタンダードになる可能性が高い
 * flaskのシンプルさとfalconの書きやすさのいいところどりをしている
 * 新しくて私のテンションが上がる（！

 実際にサンプルアプリを今回作ってみましたが、個人的にはクラスベースでHTTPのハンドラーを定義できるのが非常に気持ちよかったです。@<fn>{falcon}

== プロジェクトの構成
//emlist[project_python]{
server/
├── api.py
├── db_config.py
├── Dockerfile
├── env
├── handlers.py
├── __init__.py
├── models.py
├── nullsuckAi_model.sav // 機械学習のモデル
├── Pipfile
├── Pipfile.lock
├── services.py
├── settings.py
└── wine_attributes.csv

1 directory, 12 files
//}

Pythonは、他のJava, C#, Goなどの言語と違い、フォルダごとにパッケージを分けず、一ファイルにクラスをまとめて書くのがスタンダードです。@<fn>{package}

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
アプリが動かない、ということもないです。

なにかPythonでアプリなどを作る際は、このpipenvを利用することを強くお勧めします。@<fn>{pipenv}


=== 各モジュールの責務
#@# もしかしたら削ってもいいかも
それぞれ、modelとhandlerとservicesは次の通りの依存関係になっています。


== DBモデルを実装しよう
== コントローラを実装しよう
== 機械学習のモデルを組み込もう


//footnote[falcon][クラスベースでハンドラーを定義できるのはfalcon譲り。flaskはファンクションベースでしか定義できない。]
//footnote[package][実際にJavaみたいに分けてみましたが、いろいろ大変でした。。。]
//footnote[pipenv][しかも、pipenvとresponderは同じ作者です。すごすぎ。]