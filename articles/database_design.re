= スコアリングモデリング構築
本章では、スコアリングモデルの作り方を理解しながら、
実際にサンプルデータを使ってモデリングまで実装していきましょう。

== モデリングするにあたって大切なこと
これから一緒に作っていくスコアリングモデルは、
ユーザーの行動を採点するアルゴリズムを作るということです。

しかし、あなたが採点されるユーザーだとしたら、
その採点結果を素直に納得することはできますか。

なぜ、そのような点数になったのか、その理由は何か。
ユーザーに対してそのスコアを納得してもらえるものにする必要があります。

そのためには、モデリングする前に、
データに対してモデルを作る側が前提として
頭に入れて置かなければいいけないことがあります。

それが、@<kw>{データドリブン, Data-Driven}と@<kw>{データインフォームド, Data-Informed}という考え方です。

=== データドリブン(Data-Driven)
データドリブンとは、@<kw>{データ分析を通して意思決定を行う}スキル・文化を意味します。
データドリブンである組織は、社内にダッシュボードを内製化することによって、
データ（事実）を根拠として、意思決定の正しさと決断スピード向上の両立を目指します。

=== データインフォームド（Data-Informed）
データインフォームドとは、
そのデータは@<kw>{あくまでも目的を実現する方法を指し示す一部である}という前提を
持って、データと接することを意味します。

特にスコアリングモデルでは、この@<kw>{データインフォームド的思考}が大切です。
ユーザーである人間を判断するために収集したデータは、必ず人間の意図が介在しています。
そのデータの因果関係や正しさに適切な懐疑心をもって接することが重要です。

つまり、自分のなかの@<kw>{バイアス,bias}に惑わされないように
データの背景を想像して、倫理観を持ってデータと向き合うことが大切ということです。

== アルゴリズムとモデルの違い
また、AIを開発するにあたってカタカナ語にたくさんぶち当たります。
ですので、ここで簡単に整理します。

=== アルゴリズム
私の大好きな本のひとつ『世界でもっとも強力な9のアルゴリズム』に
//quote{
アルゴリズムとは、問題を解決するために必要単手順を正確に規定したレシピである。
//}
と書かれています。

私たちは、機械学習のアルゴリズムを学ばなくても、アルゴリズムに触れているのです。
例えば、小学校の頃に学んだ「足し算」はアルゴリズムです。

足し算は
//quote{
まず、2つの数値の1の位を足し算する。
計算結果の1の位を下に書き出す。10の位があれば、繰り上がりとして左の欄に書き出す。
次に10の位を足し算する。このとき、前の計算でくの繰りがりも忘れずに足す・・・
//}
この作業を繰り返すアルゴリズムです。

アルゴリズムのポイントは
 * 1つ1つの手順が、絶対的にに正確であること
 * 入力された値が何であれ、必ず機能すること

これはコンピュータが、
プログラムとしてアルゴリズムを使うために必要な要素だからです。

また、機械学習アルゴリズムと指す場合は、
「教師あり学習」「教師なし学習」「強化学習」などを意味します。
そして、その技法（あるいは手法）として、「決定木学習」「ニューラルネットワーク（NN）」など
たくさんの手法が存在します。

「決定木学習」をアルゴリズムとするのか、あくまでも、技法（分析手法）とするのかは、人によってまちまちです。

=== モデル
一般にモデルと呼ばれる場合は、
@<kw>{学習済モデル}や@<kw>{アルゴリズムモデル}を指すことがほとんどでしょう。
どちらを指すにしても、モデルは@<kw>{何かしらを別のモノで表現したもの}です。

例えば、芝麻信用におけるスコアリングモデルは、
@<kw>{中国でアリババ系サービスを利用するユーザーの信用力を、5つの項目を使って数値で表現したもの}です。


スコアリングモデルというのは、
@<kw>{何かの状態（目的）を、機械学習アルゴリズムを使って、別のモノで表現した}ものです。
そして、機械学習アルゴリズムにはいくつもの種類があるので、
どんなデータに対してどのようなアルゴリズムを使ったのかによってモデルは全く別のものになる可能性を持っています。

== 統計モデルと機械学習モデル
=== 統計モデル
=== 機械学習モデル

== スコアリングの種類
=== 予測スコアリング
=== リードスコアリング

== スコアリングに使用するアルゴリズム
=== 回帰分析

== スコアリングモデル構築
=== 目的設定あああああああああ
=== データセットああああああああ



== AI化って何ぞ
AIについても簡単に振り返ってみましょう。AIというと何をイメージしますか？

人間しかできなかった機能を機械化できたという点では、「目」＝画像認識　「口」＝自然言語処理　「耳」＝音声認識がわかりやすいです。しかし、今回ご紹介するスコアリングは昔からあった伝統的な統計学的考え方をビッグデータ分析によってモデル化（AI化／自動化）するお話です。

つまり、人間の「目」の代わりになったり「口」の代わりになるのではなく、「脳」＝判断の簡便化が目指す領域です。その脳を綺麗にする作業がAI化に繋がるデータベース設計です。
 * 機械が勝手にやってくれる（自動化）
 * 大量のデータを元に分析した結果がわかる（データドリブン）
 * 予測／分類／推定…できる（意思決定の補助）

== 回帰分析とは
回帰分析とはひところで言うと、複数の項目をまとめて使った分析手法の１つです。統計学ではいろいろな分析手法があります。何かを予測したいときに、それに影響を与える要因は１つだけとは限りません。

そういう場合に、複数の項目を使って予測を行うときに@<kw>{回帰分析（Regression Analysis）}という統計手法を活用します。ちなみに、イメージのデータテーブルは下記のようなものになります。

【ここに図がはいる】

このように複数の変数（項目のことを変数と呼ぶ）と多くのデータ行数を対象分析を行う手法を、@<kw>{多変量解析（nultivariate Analysis）}と呼びます。回帰分析は多変量解析の１つです。

== データ準備
 * 何を知りたいのか、分析の目的を明確にする
 * 分析の目的を明確にすると、分析不法が決まる
 * 分析手法が決まると、必要なデータの型が決まる
 * データの型に合うように、分析用データを集める

== スコアリングとはaaaaaa
スコアリングとは、「購入あり／なし」「来場あり／なし」のように、
結果が2値で表せる事柄について、いずれかの結果が得られる「確率」を予測することです。

今夏期のスコアリングには、「ロジスティック回帰」の手法が用いられています。
@<br>{}
例）1人1人の顧客の購入確率を予測する
DMを送った場合に、より購入しやすい顧客を一人一人予測しランキングすることができ、
買いやすい人から優先的にDMなどの施策をおこなうことができる。
@<br>{}
すべてのプロスペクト（見込み顧客）にアプローチする時間あるいは予算がない場合に、
リストにスコアつけることが意味を持つ。

もしある程度の人々を対象から外す必要があるならば、
最も反応しそうもない人を外すべきである。
出典：「データマイニング手法」

DM施策の最適化における活用例
マーケティング施策として広く行われているDM施策を例に、
スコアリングで「送付対象」を検討してみましょう。
DM施策において、目標とする反応率（レスポンス）を得る
ために、マーケターは送付対象を最適化するための様々な分
析を行います。
一般にはRFM（※）分析や、BIツールによる分析（顧客属性、
購買履歴、キャンペーン履歴などを用いた集計・可視化）が
行われます。
※RFMとは、下記の3要素を指す。
「最終購買日（Recency）」
「購買頻度（Frequency）」
「累計購買金額（Monetary）」

== RFM分析・BIツールにおける「課題」
購買に関するRFMの指標を用いることでロイヤリティの高い
顧客を抽出できます。しかし、継続的にDM施策を行うこと
により送付対象となる顧客が固定化し、施策回数を重ねるご
とに反応しやすい顧客の刈取りが進むため、次第に反応率は
鈍化していきます。
RFMに加えて、顧客属性（性別、居住地、年収、etc…）など
の様々な指標も含めた分析・可視化を行う場合はどうでしょ
うか。検討すべき軸が増えれば増えるほど分析に時間がかか
り、また検討すべき内容も複雑になるため、最適な送付対象
を推定するためには多くの時間とマーケター自身のスキルや
判断力が求められます。
さらには、様々なデータ項目について、グラフによる可視化
を行い仮説立てを行うことはできますが、例えば「購入金
額」「収入」「性別」…などの異なるデータ項目間で、それ
ぞれの項目がどの程度購買に影響しているかを比較し、定量
的に把握することは難しく、真に見込みの高い顧客を推定す
ることは困難です。

== より見込みの高い顧客を「1人1人」予測 - 施策効果を最大化 -

顧客に紐づく様々なデータ（顧客属性、購買に
関する指標など）を用いて、データマイニング
のアプローチを行うことによって、「個人」単
位での反応率を定量的に求めることができま
す。
=== 多変量解析
複数の対象（サンプル）について 複数の変数（カテゴ
リー）による測定値に対し、これらのデータを独立に
扱わずに、各変数間の関連性を調べる統計手法です。

== 「データマイニング」による、最適化
顧客にひもづく様々なデータ（顧客属性、購買に関する指標
など）を用いて、データマイニング（BA）でのアプローチを
行うことによって、「個人」単位での反応率を定量的に求め
ることができます。
「セグメント」単位での送付対象抽出と比較して、より精度
の高いターゲティングが可能になります。
データマイニングでは、様々なデータ項目（多変量）を一度
に扱うことができるので、各データ項目がDMへの反応率に
どれくらい影響するのかを定量的に可視化できます。過去実
績を用いた分析モデルの適用によって、送付対象1人1人の反
応率を事前に予測することが可能になります。

== スコアリングの実行
個人単位での予測は、データマイニングのスコアリング（ロ
ジスティック回帰分析）を行うことで実現できます。
顧客に紐づく様々なデータ（顧客属性、購買に関する指標な
ど）を用いたスコアリングによって、個人単位での反応率を
定量的に求めることができます。
「セグメント」単位での送付対象抽出と比較して、より精度
の高いターゲティングが可能になります。
スコアリングでは、様々なデータ項目（多変量）を一度に扱
うことができるので、各データ項目がDMへの反応率にどれ
くらい影響するのかを定量的に可視化できます。過去実績を
用いた分析モデルの適用によって、送付対象1人1人の反応率
を事前に予測することが可能になります

== 目的設計
== データセット
== カウント・集計
== 中間テーブル作成
== データ分析
=== 探索的分析
=== 探索的分析あああ2
== モデリング
=== アルゴリズム選定
=== 精度検証
=== エクスポート
== 今回は与信の代わりに、ワインをスコアリングしてみよう
信用スコアを算出するためには、個人のクレジットカード支払履歴やECでの購買データが必要です。いきなり、そのようなデータセットをランダムに用意し分析することは難しいので、今回は一般に公開されている有名なワインのデータセットを用いて予測モデルを作っていきましょう。

=== データセットあ
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
