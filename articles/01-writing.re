= この本の執筆環境について

  * Github
  * Re:View
  * wercker（CI）

この本ではGithubとRe:Viewを主軸として執筆しています。エンジニアに馴染みやすく多人数での執筆に向いています@<fn>{solo}。技術書典で出展されている同人誌ではよく見られる構成です。まずは執筆環境の概要を説明します。
//footnote[solo][もちろん個人の執筆にも向いています。]

== 概要説明

エンジニアが楽をすることに特化した環境です。面倒なことは全て丸投げしちゃいましょう。

=== git / Github

gitは今エンジニアがもっともよく使っているバージョン管理ツールです。Linuxの作者Linus氏が、Linuxの開発に耐える大規模バージョン管理ツールが無いことにブチ切れて開発したものです。gitより以前はSubversionというソフトがよく使われていましたが、圧倒的にgitの方が使い勝手がよくあっという間に普及しました。

git さっぱりわからないという人は、わかばちゃんと学ぶ Git使い方入門を読んでみましょう。
//footnote[wakaba][@<href>{https://www.amazon.co.jp/dp/B071D4D6XX/}]

おおざっぱにいうとソースコードをタイムマシン的にバックアップして管理するものです。これを履歴といいますが、gitでは差分を省スペースで保持する仕組みなので、容量を気にせずソースコードの履歴を残せます。

====[column] ファイル管理をシステム化する利点

@<tt>{スケジュール.backup.バージョン1.最新版.old.txt}のようなファイルを見かけたことはありませんか？ファイルをコピーしてファイル名で履歴管理しようとする陥る罠です。どれが最新なのかわからないという問題があります。解決方法として@<tt>{スケジュール.2017-11-11.txt}という命名ルールで縛ろうとしてみます。この場合更新頻度が一日一回におさまればいいですが、同じ日に更新しようとした場合さらにルールが必要になります。たとえば@<tt>{スケジュール.2017-11-11.01.txt}でしょう。ですが、同時に別々の人が更新しようとした場合にはどうなるでしょうか？それに、いちいち人間が手で日付いじるの面倒じゃないですか？

そこでバージョン管理ツールです。たとえばgitでやるのならば、gitに@<tt>{スケジュール.txt}を登録するだけです。コミットという作業をするだけで勝手にソースコード貯蔵庫（リポジトリ）に保管されます。いつの時点の@<tt>{スケジュール.txt}も探し出せます。ログ参照・グラフ参照・検索などさまざまな方法で楽かつ高速で探し出せます。

ファイル名で管理するという原始的なことをする時代ではなくなったのです。

====[/column]

そしてGithub@<fn>{github}はgitを活用した世界最大のソースコードSNSで、エンジニアにとっては名刺代わりといってもいいものです。
//footnote[github][@<href>{https://github.com/}]

Githubは多人数でソースコードを同時開発するのにとても向いている便利なサービスです。別にソースコードといってもプログラミングに限定する必要はありません。本の原稿もソースコードです。そのソースコードをGithubに預けるのです。自分のPCが吹っ飛んでもGithubにソースコードを置いてる限り悲嘆する必要はありません@<fn>{local-commit}。
//footnote[local-commit][ローカルでの変更点をgithubに送ってない場合はもちろん悲しいことになります]

=== Re:View

Re:VIEW@<fn>{Re:VIEW}は現時点ではkmuto@<fn>{kmuto}さん他40人以上の開発者が参加するオープンソースな組み版ソフトです。実際にいくつもの出版社が採用しているプロユースのソフトですが、技術系同人誌でもよく使われるなど、プロもアマチュアも使うとても便利なソフトです。

//footnote[Re:VIEW][@<href>{http://reviewml.org/}]
//footnote[kmuto][@<href>{https://github.com/kmuto}]

//quote{
    Copyright © 2006-2016 Minero Aoki, Kenshi Muto, Masayoshi Takahashi, Masanori Kado.
//}

Re:VIEWでは、ASCIIで使われていた画期的な電子出版（Editor's Work Bench）の書式をベースにしつつ簡易化した記述方法で本を書きます。Markdownや他のWikiとは違う少し癖があるように感じられる記法ですが、組み版に特化していて必要十分な機能を持っています。

おおまかに分けて、行単位の命令と文章の中に入れるインライン命令で構成されています。

//listnum[list-test][リストのテスト]{
#@mapfile(sample/01-writing/hoge.re)
= Re:View はこんな感じでかきますよー の章

== ほげー

ふがー@<code>{@}<fn>{fuga}
//footnote[fuga][ふがー]

ぴよー

@<code>{#@#} コメント
#@end
//}

#@# リストの中でもRe:Viewのインライン命令を解釈してしまうので要注意
#@# #@ で始まる行に関しても、プリプロセッサの都合があるので、こんな感じに

@<code>{=} で始まる行は見出しです。特に@<code>{=}のようなイコール記号が1つのものは章です。

  * @<code>{=} 章
  * @<code>{==} 節
  * @<code>{===} 項
  * @<code>{====} 段
  * @<code>{=====} 小段

@<code>{//} で始まる行はブロック命令です。

@<code>{@}<code>{fuga}のような命令はインライン命令です。

=== wercker

Re:Viewで書かれた原稿はPDFやepubなどの形式として出力（コンパイル）しないと本にはなりません。Re:ViewというのはRe:Viewの記述方法で書かれた原稿をPDFやepubに変換するソフトです。

手元で自前でデータを出力してもいいのですがどうせならこれも自動化したいですよね。Githubに登録された原稿が自動でコンパイルされてPDFなどに変換されていれば最高じゃないですか。しかもデータの出力だけじゃありません。自動で校正するツールもあります。そういったツールで「この原稿ちょっとマズいんでは？」という指摘を全自動で出すことも簡単です。

== 環境整備

#@# これ、別の章に持って行く方がよい？

=== サーバー側

=== 個人側

====[column] こらむ

個人的には執筆環境については、VSCode + Re:Viewプラグインをオススメしておきます。Atomはもっさり感が…。

====[/column]
