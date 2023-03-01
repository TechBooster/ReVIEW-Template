# Re:VIEW Template

このリポジトリはRe:VIEW用の書籍テンプレートです。Re:VIEWバージョン5.5に対応します。
メンテナンスはTechBoosterが行っています。

 * [B5紙面サンプル（PDF）](https://github.com/TechBooster/ReVIEW-Template/tree/master/pdf-sample/TechBooster-Template-B5.pdf)
 * [A5紙面サンプル（PDF）](https://github.com/TechBooster/ReVIEW-Template/tree/master/pdf-sample/TechBooster-Template-A5.pdf)
 * [B5紙面電子書籍サンプル（PDF）](https://github.com/TechBooster/ReVIEW-Template/tree/master/pdf-sample/TechBooster-Template-ebook.pdf)

## このテンプレートのビルドの仕方は？

細かい準備(TeX入れたり)は[『技術書をかこう！』](https://github.com/TechBooster/C89-FirstStepReVIEW-v2)に準じます。

TeXの導入をしない場合でもHTMLやEPUBの出力は可能です。Dockerコンテナでビルドすることもできます。

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

## PDF出力する

rbenvやnodebrewを利用してRubyやNode.jsの準備をしている前提です。
もしそうではない場合、適宜sudoを補うこと。
Node.jsは4.x系以上が必須です。

graphvizを使ったのでbrew install graphvizとかが必要かもしれません。

```
$ gem install bundler
$ git clone git@github.com:TechBooster/ReVIEW-Template.git
$ cd ReVIEW-Template
$ npm install
$ npm run pdf
```

### Re:VIEWコマンドでPDFを出力する

セットアップが完了していれば `npm run pdf` のかわりに
```
$ cd ReVIEW-Template/articles
$ rake pdf
```

または

```
$ cd ReVIEW-Template/articles
$ review-pdfmaker config.yml
```

でも生成できます。

## EPUB・Webページ・テキストの出力

PDF以外に、EPUB・Webページ・テキストを出力することもできます。

### EPUBを出力する

```
$ npm run epub
```

または

```
$ cd ReVIEW-Template/articles
$ rake epub
```

または

```
$ cd ReVIEW-Template/articles
$ review-epubmaker config.yml
```

### Webページを出力する（webrootフォルダ）

```
$ npm run web
```

または

```
$ cd ReVIEW-Template/articles
$ rake web
```

または

```
$ cd ReVIEW-Template/articles
$ review-webmaker config.yml
```

### テキストファイルを出力する

```
$ npm run text
```

または

```
$ cd ReVIEW-Template/articles
$ rake text
```

または

```
$ cd ReVIEW-Template/articles
$ review-textmaker config.yml
```

## Dockerを使う

TeXの環境構築が困難な場合、一式セットアップ済みのDockerイメージを [ghcr.io](https://github.com/vvakame/docker-review/pkgs/container/review) または [DockerHub](https://hub.docker.com/r/vvakame/review) に用意してあるので使ってください。
Dockerがうまく動くようになっている場合、以下のコマンドで細かい準備なしにビルドを行うことができます。

```
$ docker pull ghcr.io/vvakame/review:5.5 (ghcrからのダウンロードの場合)
$ docker pull vvakame/review:5.5 (Docker Hubからのダウンロードの場合)
$ ./build-in-docker.sh
```

## 紙面や設定ファイルの切り替え

B5やA5といった紙面サイズ、印刷用・電子用といったメディアの切り替えは、articles/config.ymlのtexdocumentclassパラメータで設定しています。

```
# B5の設定(10pt 40文字×35行) - 紙版
texdocumentclass: ["review-jsbook", "media=print,paper=b5,serial_pagination=true,hiddenfolio=nikko-pc,openany,fontsize=10pt,baselineskip=15.4pt,line_length=40zw,number_of_lines=35,head_space=30mm,headsep=10mm,headheight=5mm,footskip=10mm"]
# B5の設定(10pt 40文字×35行) - 電子版
# texdocumentclass: ["review-jsbook", "media=ebook,paper=b5,serial_pagination=true,openany,fontsize=10pt,baselineskip=15.4pt,line_length=40zw,number_of_lines=35,head_space=30mm,headsep=10mm,headheight=5mm,footskip=10mm"]
# A5の設定(9pt 38文字×37行) - 紙版
# texdocumentclass: ["review-jsbook", "media=print,paper=a5,serial_pagination=true,hiddenfolio=nikko-pc,openany,fontsize=9pt,baselineskip=13pt,line_length=38zw,number_of_lines=37,head_space=15mm,headsep=3mm,headheight=5mm,footskip=10mm"]
# A5の設定(9pt 38文字×37行) - 電子版
# texdocumentclass: ["review-jsbook", "media=ebook,paper=a5,serial_pagination=true,openany,fontsize=9pt,baselineskip=13pt,line_length=38zw,number_of_lines=37,head_space=15mm,headsep=3mm,headheight=5mm,footskip=10mm"]
```

デフォルトはB5・印刷用の設定になっていますが、たとえばA5・印刷用にしたい場合にはB5のパラメータをコメントアウトし、A5のほうを有効にします。

```
# B5の設定(10pt 40文字×35行) - 紙版 ↓これをコメントアウトし…
# texdocumentclass: ["review-jsbook", "media=print,paper=b5,serial_pagination=true,hiddenfolio=nikko-pc,openany,fontsize=10pt,baselineskip=15.4pt,line_length=40zw,number_of_lines=35,head_space=30mm,headsep=10mm,headheight=5mm,footskip=10mm"]
# B5の設定(10pt 40文字×35行) - 電子版
# texdocumentclass: ["review-jsbook", "media=ebook,paper=b5,serial_pagination=true,openany,fontsize=10pt,baselineskip=15.4pt,line_length=40zw,number_of_lines=35,head_space=30mm,headsep=10mm,headheight=5mm,footskip=10mm"]
# A5の設定(9pt 38文字×37行) - 紙版 ↓こちらを有効にする
texdocumentclass: ["review-jsbook", "media=print,paper=a5,serial_pagination=true,hiddenfolio=nikko-pc,openany,fontsize=9pt,baselineskip=13pt,line_length=38zw,number_of_lines=37,head_space=15mm,headsep=3mm,headheight=5mm,footskip=10mm"]
# A5の設定(9pt 38文字×37行) - 電子版
# texdocumentclass: ["review-jsbook", "media=ebook,paper=a5,serial_pagination=true,openany,fontsize=9pt,baselineskip=13pt,line_length=38zw,number_of_lines=37,head_space=15mm,headsep=3mm,headheight=5mm,footskip=10mm"]
```

印刷用と電子用の両方を作りたいときには、設定ファイルの継承機能を利用できます。たとえばB5電子版の設定ファイルの例をconfig-ebook.ymlとして用意しています。この内容は次のようにシンプルに既存のconfig.ymlのtexdocumentclassパラメータを置き換えているだけです。

```
# 継承元設定
inherit: ["config.yml"]

# B5の設定(10pt 40文字×35行) - 電子版
texdocumentclass: ["review-jsbook", "media=ebook,paper=b5,serial_pagination=true,openany,fontsize=10pt,baselineskip=15.4pt,line_length=40zw,number_of_lines=35,head_space=30mm,headsep=10mm,headheight=5mm,footskip=10mm"]
# A5の設定(9pt 38文字×37行) - 電子版
# texdocumentclass: ["review-jsbook", "media=ebook,paper=a5,serial_pagination=true,openany,fontsize=9pt,baselineskip=13pt,line_length=38zw,number_of_lines=37,head_space=15mm,headsep=3mm,headheight=5mm,footskip=10mm"]
```

このconfig-ebook.ymlを使ってPDFを生成するには次のようにします。

```
$ REVIEW_CONFIG_FILE=config-ebook.yml npm run pdf
```


または

```
$ cd ReVIEW-Template/articles
$ REVIEW_CONFIG_FILE=config-ebook.yml rake pdf
```

または

```
$ cd ReVIEW-Template/articles
$ review-pdfmaker config-ebook.yml
```

Docker環境でも以下のように指定できます。

```
$ REVIEW_CONFIG_FILE=config-ebook.yml ./build-in-docker.sh
```

紙版と電子版では以下のような違いがあります。

 * 紙版：印刷用に、トンボ、デジタルトンボを設置。いくつかの同人誌印刷所で要求事項となっているノドへの隠しノンブル、大扉からのアラビア数字通し。ハイパーリンクは無効化。表紙（cover）は無視。
 * 電子版：電子配布用に、仕上がり紙面サイズ。大扉からのアラビア数字通し。ハイパーリンク有効。表紙（cover）を中央合わせで配置。

### EPUB/Web出力用CSSファイルの編集方法

articles/ディレクトリ以下の各種*.scssファイルを編集し、

```
./rebuild-css.sh
```

コマンドでCSSファイルをビルドしてください。

## 過去のRe:VIEW 3・4・5依存プロジェクトを最新のRe:VIEWバージョンに移行する

Re:VIEW 3以降のプロジェクトは、review-updateコマンドで簡単に更新できます。

既存のプロジェクトフォルダ内 (本リポジトリを使っている場合はarticlesフォルダ) で、review-updateコマンドを実行してください。

```
$ review-update
** review-update はプロジェクトを 5.5.0 に更新します **
config.yml: 'review_version' を '5.0' に更新しますか? [y]/n
プロジェクト/sty/review-base.sty は Re:VIEW バージョンのもの (/var/lib/gems/2.7.0/gems/review-5.5.0/templates/latex/review-jsbook/review-base.sty) で置き換えられます。本当に進めますか? [y]/n
プロジェクト/sty/review-jsbook.cls は Re:VIEW バージョンのもの (/var/lib/gems/2.7.0/gems/review-5.5.0/templates/latex/review-jsbook/review-jsbook.cls) で置き換えられます。本当に進めますか? [y]/n
完了しました。
```

その後、articles/sty/reviewmacro.styとarticles/sty/techbooster-doujin-base.styを本リポジトリのファイルで上書きしてください。

- [reviewmacro.sty](https://raw.githubusercontent.com/TechBooster/ReVIEW-Template/master/articles/sty/reviewmacro.sty)
- [techbooster-doujin-base.sty](https://raw.githubusercontent.com/TechBooster/ReVIEW-Template/master/articles/sty/techbooster-doujin-base.sty)

## 過去のRe:VIEW 2依存プロジェクトをRe:VIEW 5に移行する

Re:VIEW 2系向けの過去のTechBoosterテンプレートは、Re:VIEW 3以降とは互換性がありません。Re:VIEWは2から3以上への移行を支援する「review-update」というコマンドを提供していますが、TechBoosterテンプレートを使用しているプロジェクトは対象外となっています。

### Re:VIEW 3以降の互換モードを使う

config.ymlで`review_version: 2.0`としておけば、当面は互換モードによりRe:VIEW 3以降でも変わりなく動作します。ただし、一部のRe:VIEW 3以降固有の機能は利用できません。また、将来的に互換は破棄される可能性があります。

### Re:VIEW 5のテンプレートに置き換える

Re:VIEW 3以降ではTeX関連のファイルが大きく変わっているため、Re:VIEW 2系のプロジェクトの既存のファイルをいったん退避し、必要に応じて設定を書き戻すという手順になります。

1. 事前にフォルダごと必ずバックアップを取っておいてください。
2. layoutsフォルダをリネーム（たとえばlayouts-oldなど）します。
3. styフォルダをリネーム（たとえばsty-oldなど）します。
4. config.ymlファイルをリネーム（たとえばconfig.yml-oldなど）します。
5. TechBoosterテンプレートのarticles/styフォルダをフォルダごとコピーします。
6. TechBoosterテンプレートのarticles/config.ymlファイルをコピーします。
7. TechBoosterテンプレートのlibフォルダをフォルダごとコピーします。
8. TechBoosterテンプレートのRakefileファイルをコピーします。
9. config.ymlに、カスタマイズしていた書名や著者名などを書き戻します。

書き戻しではなくどうしても古いconfig.ymlの書き換えで対処したいという場合は、以下の点に注意してください。

 * review_versionパラメータ：値を5.0にする必要があります。
 * texstyleパラメータ：値を["reviewmacro"]とします。
 * texdocumentclassパラメータ：2つの引数の内容は大きく変わっています。
 * texcommandパラメータ：オプションを指定していた場合、texoptionsパラメータに移設する必要があります。
 * dvicommandパラメータ：オプションを指定していた場合、dvioptionsパラメータに移設する必要があります。

layouts/layouts.tex.erbやsty/techbooster-doujin.styに何らかのカスタマイズを加えていた場合は、sty/review-custom.styに類似の実装をする必要があります。Re:VIEW 2系のTechBoosterテンプレートはマクロ定義時点から書き換える手法をとっていましたが、Re:VIEW 3系から`\renewcommand`・`\reenvironment`・`\def`などの命令を使って既存のマクロ定義を再定義するというやり方に変わっています。

### Re:VIEW 3以降の変更点について

Re:VIEW 3以降で変わったことの詳細については、以下を参照してください。

* [Re:VIEW 5.5 での変更点](https://review-knowledge-ja.readthedocs.io/ja/latest/releases/review550.html)
* [Re:VIEW 5.4 での変更点](https://review-knowledge-ja.readthedocs.io/ja/latest/releases/review540.html)
* [Re:VIEW 5.3 での変更点](https://review-knowledge-ja.readthedocs.io/ja/latest/releases/review530.html)
* [Re:VIEW 5.2 での変更点](https://review-knowledge-ja.readthedocs.io/ja/latest/releases/review520.html)
* [Re:VIEW 5.1 での変更点](https://review-knowledge-ja.readthedocs.io/ja/latest/releases/review510.html)
* [Re:VIEW 5.0 での変更点](https://review-knowledge-ja.readthedocs.io/ja/latest/releases/review500.html)
* [Re:VIEW 4.2 での変更点](https://review-knowledge-ja.readthedocs.io/ja/latest/releases/review420.html)
* [Re:VIEW 4.1 での変更点](https://review-knowledge-ja.readthedocs.io/ja/latest/releases/review410.html)
* [Re:VIEW 4.0 での変更点](https://review-knowledge-ja.readthedocs.io/ja/latest/releases/review400.html)
* [Re:VIEW 3からのLaTeX処理](https://review-knowledge-ja.readthedocs.io/ja/latest/latex/review3-latex.html)

### Re:VIEW 2用のテンプレートの入手

何らかの理由でどうしてもRe:VIEW 2系のTechBoosterテンプレートが必要なときには、以下のリリースページから「review-2.4」リリースアーカイブファイルを利用してください。

* https://github.com/TechBooster/ReVIEW-Template/releases

## 権利

 * 設定ファイル、テンプレートなど制作環境（techbooster-doujin-base.styなど）はMITライセンスです
   * 再配布などMITライセンスで定める範囲で権利者表記をおねがいします
   * 本設定を使って生成した書籍は現段階のRe:VIEWではソフトウェアたりえません。したがってライセンスは発生しません（あとがきなどへの表記はあると嬉しいものの生成物での表記は不要です）
 * articles/styにあるファイルには以下のライセンスが適用されています。
   * review-jsbook.cls, review-base.sty, review-style.sty, review-custom.sty: MIT License
   * jumoline.sty: The LaTeX Project Public License
   * plistings.sty: MIT License
   * gentombow.sty: BSD License
   * jsbook.cls: BSD License
