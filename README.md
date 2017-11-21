# ワンストップ！技術同人誌を書こう(仮)

## 概要説明
このリポジトリは[親方Project活動記録 C93合同誌：「ワンストップ！技術同人誌を書こう\(仮\)」　の企画要旨説明](http://oyakata2.blog104.fc2.com/blog-entry-13.html)な同人誌のものです。

参加いただける方は、[@oyakata2438](https://twitter.com/oyakata2438)までご連絡ください。

## この本の最終目的
技術書典およびコミケ、その他で、技術書（技術系同人誌）を書いてみたい人が、「これ一冊で」本を作ってイベントで配布するまでの基本的な流れが全て把握可能な、マニュアル的同人誌を作る。

C93に配布予定でして、「購入者がそれを参考にして4月開催（かも）の技術書典4に参加する」なんてのがあるとステキですよね。

というわけで、皆さんの同人活動におけるノウハウを教えてください。

執筆方法は…Githubのブランチをforkして、各々コミットしてください。
プルリク上げてもらうか、メンバーなら直接PushでもOKです。
章追加は、catalog.ymlに追記してください。（追記しないと、pdfに組み込まれませんよｗ）

不明点があれば、issueかツイッターあたりで誰かに聞くと、誰かが答えてくれるかも。

ここをこうした方がいい、とか、提案は大歓迎（よろしくお願いします・・・）

## 執筆・配布スケジュール（逆算）
12月10日：原稿確定→入稿（11日夜予定）

12月5日頃：個別原稿一次締め切り→クロスレビュー

12月1日頃：もくじ確定

11月1日　執筆者募集開始→目次は順次集約（執筆者が適宜追加）

## 筆者紹介兼あとがき
Contributorsに、自己紹介と、執筆者コメントを忘れずに入れてください。HPとか、ツイッターとか、GitIDとか、自由に入れてください。またこの企画に参加してくれた理由とか、今後の技術書クラスタに期待すること、自分の活動の振り返りでも、なんでもありです。5行～10行くらいでお願いします。

## 執筆にあたってのお願い
ネタがかぶらないよう/かぶってもよいようにするため

（1）早めに目次にコミットする（執筆者の皆様へのお願い）

（2）かぶっても「うちはこうしている」という形で残す（コラム化する・別節にするなど）

として、ボツにしなくてすむようにしたいです。

もし既存の項目があって、あえて「うちはこうしている」と明示的にやりたい場合は、その旨表明いただければ、なんとかします。

## 今のところの参加者（11/22）

親方

@erukitiさん

せとあずさん

もふもふさん

なかひこくん

病葉さん

ゆきさん

Kauplanさん

湊川さん
　（把握している限り&順不同）適宜追加します…

## CI

[![wercker status](https://app.wercker.com/status/7193689c89e58981853142a1e3d685b1/s/master "wercker status")](https://app.wercker.com/project/byKey/7193689c89e58981853142a1e3d685b1)

werckerを使っています。リポジトリが更新されると自動でPDFが出力されます。

* https://app.wercker.com/onestop-techbook/c93-onestop-techbook/runs

## このテンプレートのビルドの仕方は？

細かい準備(TeX入れたり)は[『技術書をかこう！』](https://github.com/TechBooster/C89-FirstStepReVIEW-v2)に準じます。

ベースには、[TechBooster/ReVIEW\-Template: TechBoosterで利用しているRe:VIEWのテンプレート（B5/A5/電子書籍）](https://github.com/TechBooster/ReVIEW-Template) を使っています。


### Dockerを使う方法

Dockerを使うのが一番手軽です。

```sh
$ docker run --rm -v `pwd`:/work vvakame/review /bin/sh -c "cd /work/articles ; review-pdfmaker config.yml"
```

articles/c93-onestop-techbook.pdfが生成されます。

### RubyやNode.jsをなんとかインストールして

```sh
$ npm run global
$ npm install
```

を叩くとインストールが完了してたり、

```sh
$ npm run pdf
```

Dockerの時同様に、articles/c93-onestop-techbook.pdfが生成されます。

-----

## Re:VIEWの使い方は？

[技術書をかこう！?はじめてのRe:VIEW?改訂版](https://github.com/TechBooster/C89-FirstStepReVIEW-v2)
のリポジトリで解説しています。Re:VIEWに対応した執筆ノウハウ本です。
Re:VIEWの使い方と制作にあたっての企画?執筆?編集?告知?会場頒布までのノウハウをギュッと詰めています。

https://techbooster.booth.pm/ でPDF版の販売予定があります

## 環境の準備

本書は[Atom](https://atom.io/)で[language-review](https://atom.io/packages/language-review)と[linter-redpen](https://atom.io/packages/linter-redpen)を導入している前提です。
内部的に[RedPen](http://redpen.cc/)を利用するため、 `brew install redpen` とかしておいてください。

Atom以外の環境の場合、[prh](https://github.com/vvakame/prh)または[reviewjs-prh](https://github.com/vvakame/reviewjs-prh)とRedPenを自力でなんとか頑張って適用してください。

警告やlintのエラーを放置して編集に突入してしまうと @mhidaka が過労で死にます。
RedPenは試験的に導入したため、知見やルールについてまだ固まっていません。
意見・感想・修正案などあれば @mhidaka までご相談ください。

### PDF出力する

rbenvやnodebrewを利用してRubyやNode.jsの準備をしている前提です。
もしそうではない場合、適宜sudoを補うこと。
Node.jsは4.x系以上が必須です。

graphvizを使ったのでbrew install graphvizとかが必要かもしれません。

```
$ gem install bundler
$ git clone git@github.com:TechBooster/ReVIEW-Template.git
$ cd ReVIEW-Template
$ git submodule init
$ git submodule update
$ npm install
$ npm run pdf
```

### Re:VIEWコマンドでPDFを出力する

セットアップが完了していれば `npm run pdf` のかわりに
```
$ cd ReVIEW-Template/articles
$ review-pdfmaker config.yml
```

でも生成できます。

### Dockerを使う

TeXの環境構築が困難な場合、一式セットアップ済みの[dockerイメージ](https://registry.hub.docker.com/u/vvakame/review/)を用意してあるので使ってください。
Dockerがうまく動くようになっている場合、以下のコマンドで細かい準備なしにビルドを行うことができます。

```
$ docker pull vvakame/review
$ ./build-in-docker.sh
```

### 権利

 * 設定ファイル、テンプレートなど制作環境（techbooster-doujin.styなど）はMITライセンスです
   * 再配布などMITライセンスで定める範囲で権利者表記をおねがいします
   * 本設定を使って生成した書籍は現段階のRe:VIEWではソフトウェアたりえません。したがってライセンスは発生しません（あとがきなどへの表記はあると嬉しいものの生成物での表記は不要です）
