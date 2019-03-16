={python} PythonでAPIを作成しよう
第3章のデータ分析やデータベースへのアクセスをPythonのAPIとして実装していきましょう。

== 今回、利用する構成
今回、API側では、次の構成を利用します。

//table[kousei][今回の構成]{
名称  説明
-----------------------
APIフレームワーク	responder
ORM	　SQLAlchemy
機械学習ライブラリ	scikit-learn
//}

=== APIフレームワーク Responder
PythonのフレームワークというとDjango, Flaskが有名ですが
今回は、responderというフレームワークを利用します。
responderは2018年10月に公開された、かなり新しいフレームワークです。

今回、なぜresponderを扱うのか、理由は次のとおりです。
 * responderがPythonのデファクトスタンダードであるrequestsやpipenvの作者であるKenneth Reitzさんによって作られており、今後flask等に代わりPythonのapiフレームワークのデファクトスタンダードになる可能性が高い
 * flaskのシンプルさとfalconの書きやすさのいいところどりをしている
 * 



== APIのルーティング設計
== コントローラを実装しよう
== モデルを実装しよう
== ロジスティック回帰を実装しよう