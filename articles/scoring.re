= スコアリングを考えよう

== スコアリングとは
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
