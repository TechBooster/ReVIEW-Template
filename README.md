技術書典13へ向けた執筆用repositoryです。
TechBoosterさんのTemplateを元に作成しています。
TechBoosterさん、ありがとうございます。

https://github.com/TechBooster/ReVIEW-Template

# スケジュール
* 2022/08/01(月): タイトル締切
* 2022/08/15(月): アウトライン締め切り
* 2022/08/29(月): 本文締め切り
* 2022/09/02(金): グリー広報法務審査締め切り
* 2022/09/08(木): 技術書典運営審査締め切り
* 2022/09/11(日): 技術書典13 開催日

# 用語解説
* タイトル締め切り: タイトルを書いたPullRequestを作ってください。例: https://github.com/greebookfestclub/bookfest9/pull/1
* 本文締め切り: 本文を書いたPullRequestを作ってください。例: https://github.com/greebookfestclub/bookfest7/pull/21

# 作業環境
原稿管理は、GitHubで行います。
上記の例を参考にPullRequest作成をお願いします。
分かりづらいようでしたら、mentionいただければサポートします。
PullRequestを作るとCircleCIでPDFが自動的に作成されます。
PDFを確認するためには、GitHubのgreebookfestclub organizationに参加する必要があります。
捨てアカウントで構いませんので、GitHub(github.com)のアカウントを教えてください。
* 技術書典13原稿: https://github.com/greebookfestclub/bookfest13

# おすすめエディッタ
AtomをPCにインストールします。 https://atom.io/
その後、以下のコマンドでプラグインをインストールします。

```
$ brew install redpen
$ apm install language-review
$ apm install linter-redpen
```

# 記事の追加方法
articlesの下に"名前.re"というファイルを作り、review形式で記載します。
作ったファイル名をarticles/catalog.ymlに追加します。

* review形式説明: https://github.com/kmuto/review/blob/master/doc/format.ja.md
* re:view書式完全版: https://github.com/kmuto/review/blob/master/doc/format.ja.md

# 手元でのbuild方法
build-in-docker.shを実行すると、articles/GREE-BookFest13.pdfができます。
また、PullRequestを作るとCircleCIでBuildされます。

```
$ git clone git@github.com:greebookfestclub/bookfest13.git
$ cd bookfest13
$ ./build-in-docker.sh
✔ SUCCESS built GREE-BookFest13.pdf
$ ls articles/GREE-BookFest13.pdf
articles/GREE-BookFest13.pdf
```

# GitHubActionsによるPDF生成
PullRequestを作ると自動的にPDFが生成されます。
PullRequestのActionsタブからPDFを確認できます。

# review形式が苦手な方へ
review形式が苦手な方や環境構築が難しい方がいましたら、articles/名前.md というファイル名でMarkDownで書いて連絡いただければ、こちらで変換します。
