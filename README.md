# ワンストップ！技術同人誌を書こう(仮)

このリポジトリは[親方Project活動記録 C93合同誌：「ワンストップ！技術同人誌を書こう\(仮\)」　の企画要旨説明](http://oyakata2.blog104.fc2.com/blog-entry-13.html)な同人誌のものです。

ベースには、[TechBooster/ReVIEW\-Template: TechBoosterで利用しているRe:VIEWのテンプレート（B5/A5/電子書籍）](https://github.com/TechBooster/ReVIEW-Template) を使っています。

## このテンプレートのビルドの仕方は？

細かい準備(TeX入れたり)は[『技術書をかこう！』](https://github.com/TechBooster/C89-FirstStepReVIEW-v2)に準じます。

TeXの導入をしない場合でもHTML出力は可能です。Dockerコンテナでビルドすることもできます。

### RubyやNode.jsをなんとかインストールして

```sh
$ npm run global
$ npm install
```

を叩くとインストールが完了してたり、

```sh
$ npm run pdf
```

を叩くと、articles/c93-onestop-techbook.pdfが生成されます。

(当方、既にRuby, Node.jsセッティング済みMacなので、他環境の人いろいろ教えてください！)
(WindowsだとたぶんDockerコンテナ使うのが一番らくです。次点ではbash on Windows)

-----

## Re:VIEWの使い方は？

[技術書をかこう！〜はじめてのRe:VIEW〜改訂版](https://github.com/TechBooster/C89-FirstStepReVIEW-v2)
のリポジトリで解説しています。Re:VIEWに対応した執筆ノウハウ本です。
Re:VIEWの使い方と制作にあたっての企画〜執筆〜編集〜告知〜会場頒布までのノウハウをギュッと詰めています。

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
