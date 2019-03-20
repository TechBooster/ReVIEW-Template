= AI化につながるデータベース設計をしよう
== 今回は与信の代わりに、ワインをスコアリングしてみよう
信用スコアを算出するためには、個人のクレジットカード支払履歴やECでの購買データが必要です。いきなり、そのようなデータセットをランダムに用意し分析することは難しいので、今回は一般に公開されている有名なワインのデータセットを用いて予測モデルを作っていきましょう。

=== データセット
今回使用するデータセットは、2008年10月に公開された「ポルトガル北部のワイン品質データセット」です。
引用元URL: http://archive.ics.uci.edu/ml/datasets/Wine+Quality　データセットはhttp://archive.ics.uci.edu/ml/machine-learning-databases/wine-quality/から無料でダウンロードすることができます。

=== カテゴリデータの必要性
結果をどれくらいの粒度で把握したいのかによって、カテゴリデータの量が変わってきます。全体をひとまとまりで扱うのであれば不要だし、何かの基準を設けてデータを分けたいなら、そのカテゴリ分類が必要です。
ちなみに、全体のを一括りで分析するのは、大事な視点を見逃してしまう恐れがあるから、層別化して分析することは大切です。

== データへの向き合い方◯ステップ
機械学習は端的に表現すると「機械を使って統計学を利用する」ことです。では、なぜ機械を使うのかというと、「早いから（人間より機械の方が得意な作業）」です。
では、統計学を使う理由は？その理由は①自分ひとりの力で状況を把握できる②他人に説明できる説得性がある③だから、みんなに共有できるからです。

①が主にデータ分析でおこなう探索的分析。②はアルゴリズムの可視化につながり、③は社内の報告ではレポートになるでしょうし、実装すればプロダクト（デプロイ）になると考えています。


=== データの可視化
参考: 01_データセットの中身を確認しよう

=== 列名を確認
参考: 01_データセットの中身を確認しよう

ここで、ビジネス理解とデータ理解を突合させることが大切。
意味がわからない列名は、AI化することはできない。
その列にどんな意味や背景があるのか、正確に取れる値なのかそうでないのか
しっかりと精査することが大切。

=== 把握のためにやること
把握のためにやることは３つ。①基本統計量（具体的な数値を知る）②グラフ（全体感を知る）
 * データ個数の確認
 * 欠損値の確認
 * 統計量の確認
ここで、わざわざcsvファイルにエクスポートするのは面倒だし時間がかかるので、googleスプレッドシートに貼り付ける。
特に、googleスプレッドシートは翻訳関数がついているのでカラム名が英語表記である場合に日本人だと翻訳する時間を省くことができるので魅力的。

=== データクレンジングって一体なんなん
データ整形を端的にいうなら下のルールをまとめろってこと。
 * 1行に1レコードしかいれるな
 * 1列に1フィールドしかいれるな
 * 先頭行に列名（データラベル）を絶対に入れろ
 * 空白の列・行を絶対に作るな
 * 余計な情報（スペース、自由入力テキスト）をいれるな

=== 可視化
回帰分析は、相関関係がベースになっています。なのでまずは総関係について説明します。

よくあるのがグラフの使い方とグラフの変数の選び方がわからないというケース。
だから、高いBIツールなどを使いこなすことができていない。
そこを今回は回帰に必要なところに絞って解説します。



==== 正の相関と負の相関
正の相関：一方が多い（少ない）ときに、もう一方も多い（少ない）
負の相関：一方が多い（少ない）ときに、もう一方は少ない（多い）


なんでグラフにする必要があるの？→データの特徴を把握しやすいから

【相関関係の図】
散布図の作り方
 * 予測したい変数や注目したい変数を縦軸に配置する
 * 原因と結果という関係にある場合、原因となる変数は横軸。結果となる変数を縦軸にする
 * 時系列で考えたときは、先に発生する変数を横軸に、あとから発生する変数が縦軸

相関関係と相関係数
 * 相関係数は常に-1から1の間に収まる
 * 相関係数が0から1に近ければ近いほど、強い正の相関がある
 * 相関係数が0から-1に近ければ近いほど、強い負の相関がある

【相関の強さ判断テーブル表がはいります】
0.8　強い相関
0.6　相関がある
0.4 弱い相関
0.2未満　ほとんど相関がない



== モデルをつくろう！
どんなモデルを作りたいのかを整理してみましょう
【ここに図がはいる】

予測と要因分析をおこないたい！
「売り場面積が2,050平方メートルで、最寄り駅からの所要時間が21分、駐車場の収容台数が66台の場所に立地している店舗の場合、売上予測は1億1000万円です。
ほかの条件が変わらない場合、駐車場の収容台数を66台から80台にすると、売上は400万円増の1億1,400万円が見込めます」といえるロジック・説明を用意したい。

【写真にとって文章とプロセスを挿入する】

=== 最適なモデルを作るためのステップ
 * まずはすべての説明変数をつかって回帰分析を実行する
 * 影響度がもっと小さい説明変数を取り除いて、再び回帰分析を実行する
 * 説明変数が一個になるまで繰り返す
 * すべての回帰分析実行結果から、「補正R2」の欄、自由度調整済決定係数を比較して、もっとも大きな値を示した実行結果から作る回帰式を最適な回帰式とする

=== 前提
 * 絶対に当たる予測モデルは存在しない
 * 業界によって必ず（または精度良く）予測に使える分析手法（統計手法）というのは存在しない
 * 業界によって必ず予測に使える説明変数というのは存在しない

だから、一旦モデルを構築したあとに、分析手法や説明変数の見直しの機会を設けること。
そして、いきなり将来の予測ぉせずに、正解がわかっている現在手元にあるデータを基に。仮の予測を行い、精度よく予測ができることがわかった上で、将来の予測に入る。
（これが機械学習モデルを用いた教師あり学習予測モデル構築プロセス）

== データは真実を語る面と恣意的解釈が混在している
前提条件や予測に至るまでの流れを説明しよう。こんかいのワインの予測はあくまでもスペイン（要確認）の赤ワインの話であって、世界中のワインの話じゃない。
また、影響度の高い要素がAだとしてもAを変えることが困難である場合は、その要因分析は施策実行性が低い。
動き出せないなら、そのデータはゴミ。それくらい思い切ってアウトプットを意識していく。
ビジネスやサービスにAIをつかう＝限界がある。それをわかった上で扱う必要がある。そのために、できるかぎりデータを正しく整理して収集・保有し、データを正しく扱う。

===[column]
@<kw>{精度の高いDeepLearningよりも説明性の高い決定木？}
絶対に当たる予測モデルが創れないことを知ると、人間は「結果ではなく根拠を知りたがる」傾向にあります。
なので、最終的にDeepLarningで予測モデルを構築するとしても、説明性の高い分析を事前に行うほうがいいかもしれません。

説明性とは
 * どのようなデータを用意したのか
 * 予測をするための数式はどのように求めたのか
 * その予測の数式によって、予測の値はいくらになるのか




===[/column]


=== 今回は教師あり学習

=== モデルを吐き出す

第４章へ
