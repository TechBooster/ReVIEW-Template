# Markdownファイルの取り込み

[pandoc](https://pandoc.org/) および [pandoc2review](https://github.com/kmuto/pandoc2review) を使って、Markdown 形式で書かれたファイルをあたかも Re:VIEW の1つの章のように扱うことができます。

[Installing pandoc](https://pandoc.org/installing.html) に従って pandoc をまずご利用の環境にインストールしておいてください。

pandoc2review 機能を有効にするには、config.yml の contentdir パラメータを以下のように設定します。

```
contentdir: _refiles
```

そして、catalog.yml に Markdown ファイルの拡張子を `.re` に変更した名前で登録します。たとえばこのファイル自体を登録するのは以下のようになります。

```
PREDEF:
  - preface.re

CHAPS:
  - article.re
  - markdown.re

APPENDIX:

POSTDEF:
  - contributors.re
```

pandoc の Markdown 記法を逸脱しない範囲で、Re:VIEW 文法の大半を代替できます。[pandoc2review における Markdown 形式処理の注意事項](https://github.com/kmuto/pandoc2review/blob/main/markdown-format.ja.md) を参照してください。
