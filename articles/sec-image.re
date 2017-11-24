#@# -*- coding: utf-8 -*-

= 図を埋め込む（macOS編）

macOSでKeynote.appを使って図を作成し、原稿に埋め込む方法を紹介します。また印刷用に高解像度の画像を作成する方法も説明します。

「Keynote.app」とは、macOSやiOSにおいて無料で使えるプレゼンテーションソフトです。基本的な作図機能が備わっているので、以降ではKeynote.appを使った作図方法を紹介します。

なおこの章はmacOSのみを対象としています。ご了承ください。


=={sec-image-install} Keynote.appをインストールする

Keynote.appをまだインストールしてない場合は、インストールしましょう。

 1. 画面左上のアップルメニュー（リンゴのアイコン）をクリックし、「App Store...」を選びます。
 2. App Storeアプリが起動するので、右上の検索欄で「Keynote」と入力し、Returnキーを押します。
 3. 検索結果のいちばん最初に「Keynote」が出てくるので、「入手」をクリックしてインストールします。

これでKeynote.appがインストールされました。Finder.appで「アプリケーション」フォルダを見ると、「Keynote.app」があるはずです（@<img>{keynote-installed}）。

//image[keynote-installed][「アプリケーション」フォルダに「Keynote.app」がインストールされる]{
//}


=={sec-image-draw} Keynote.appで作図する

Keynote.appを使って、図を描いてみましょう。

 1. Keynote.appを起動し、テーマ「ホワイト」を選択してスライドを新規作成します。
 2. ウィンドウ右上の「フォーマット」タブ→「マスターを変更」ボタンを押し、「Blank」を選びます。これで白紙の状態になりました。
 3. 試しに、何か適当な図を描いてみましょう。ウィンドウ上部にある「図形」アイコンをクリックし、適当な図形を選んで、位置やサイズを変更します。ここでは@<img>{keynote-screenshot}のような図を描いてみました。
 4. Command-Sを押して、スライドを保存します。保存先はプロジェクトと同じフォルダ、ファイル名は「images.key」がいいでしょう@<fn>{fn1}。

//footnote[fn1][Re:VIEWで使う画像は「images」フォルダに入れるので、「スライドはimages.key、画像保存先はimages」というルールが分かりやすいでしょう。]

//image[keynote-screenshot][Keynote.appでの作図例][scale=1.0]{
//}


=={sec-image-save} 図をPNG画像として保存する

Keynote.appでは、メニューから「ファイル→書き出す...」を選ぶと、スライドを画像に変換できます。しかし、この機能はスライドの見た目をそのまま画像に変換するための機能であり、PDFに埋め込む図を作成する目的には向いていません。

PDFに埋め込む図を作るには、次の手順がいいでしょう。

 1. Keynote.appの画面で、Command+Aを押して図形をすべて選択し、Command+Cでクリップボードにコピーします。
 2. Preview.appを起動し、Command+Nを押すと、先ほどコピーした内容を使って画像が作成されます（@<img>{keynote-newimage}）。
 3. そのままCommand+Sを押して、画像を保存します。保存先は「images」@<fn>{images}、フォーマットは「PNG」、ファイル名はたとえば「example1.png」にします。

//image[keynote-newimage][Preview.appでCommand+Nを押すと、クリップボードの内容が画像になる][scale=0.5]{
//}

これで画像が作成されました。この画像をRe:VIEWの原稿に埋め込むには、@<list>{review-image1}のようにします。ここで「@<code>{[example1]}」は拡張子を除いた画像ファイル名、「@<code>{[約束された...]}」の部分は図の説明文です。詳しくは「Re:VIEW フォーマットガイド」@<fn>{review-format-guide}を参照してください。

//list[review-image1][Re:VIEWの原稿に画像を入れるためのテキスト]{
  //image[example1][約束された勝利のサイクル]{
  //}
//}

//footnote[images][Re:VIEWでは、デフォルトでは「images」というフォルダに画像を入れることになっています。]
//footnote[review-format-guide][https://github.com/kmuto/review/blob/master/doc/format.ja.md]

実際に埋め込んでみると、@<img>{example1}のようになります。思ってたよりも大きく表示されてしまいますね。

//image[example1][約束された勝利のサイクル]{
//}


=={sec-image-width} 画像の幅を設定する

Re:VIEWの原稿に画像を埋め込むと、思ったより大きく表示されてしまいました。これを解決するには、2つ方法があります。

1つ目は、Re:VIEWで画像の倍率を設定する方法です。たとえば@<list>{review-image2}のように「@<code>{[scale=0.5]}」を追加すると、0.5倍（つまり半分）の大きさで画像が埋め込まれます。

//list[review-image2][Re:VIEWの原稿に画像を入れるためのテキスト]{
  //image[example1][約束された勝利のサイクル][scale=0.5]{
  //}
//}

実際に埋め込んでみると、@<img>{example2}のようになります。画像の大きさが半分になっていることが分かります。

//image[example2][約束された勝利のサイクル（scale=0.5）][scale=0.5]{
//}

2つ目は、透明な四角形を背景に埋め込むことで、画像そのものの大きさを変更する方法です。具体的には次のようにします。

 1. ウィンドウ上部の「図形」アイコンから四角形を選びます。
 2. 四角形を、図形を覆うくらいの位置と大きさに変更したあと、Commad+Shift+Bを押して最背面に移動させます。
 3. ウィンドウ右上の「フォーマット」アイコン→「スタイル」タブを選びます。「塗りつぶし：塗りつぶしなし、枠線：枠線なし、シャドウ：シャドウなし」に変更します。
 4. ウィンドウ右上の「フォーマット」アイコン→「配置」タブを選びます。サイズを、たとえば「幅：900pt、高さ：300pt」に変更します（@<img>{keynote-bgbox}）。幅と高さは、紙面の幅と図形の高さに合わせて調整してください。

//image[keynote-bgbox][図形の最背面に透明な四角形を置くことで、画像の幅と高さを指定する][scale=1.0]{
//}

これで、画像の幅と高さを指定できました。ここまでできたら、画像に変換し直します。具体的には、Keynote.appでCommand+AとCommand+Cを押し、Preview.appに切り替えてからCommand+NとCommand+Sを押します。

実際に原稿に埋め込むと、@<img>{example3}のようになりました（「@<code>{[scale=0.5]}」は指定していません）。画像の倍率を指定しなくても、ほどよい大きさで表示されていることが分かります。

//image[example3][約束された勝利のサイクル（900x300）]{
//}

どちらの方法でもうまくいきますが、個人的には2つ目の方法を勧めます。実際に試したところ、1つ目の方法では画像ごとに適切な倍率を指定するのが面倒でした。2つ目の方法だと作図している段階で大きさが分かるので、楽でした。


=={sec-image-dpi} 印刷用の画像に変換する

実は、このままの画像では解像度が低すぎて、印刷には向きません。

画像の解像度が低いことを確認してみましょう。Re:VIEWの原稿を「@<code>{rake pdf}」コマンドでコンパイルし、PDFを作成してPreview.appで表示します。そしてCommandキーを押しながら「+」キーを3、4回押してみてください。押すたびにPDFが拡大表示されることが分かります。PDFに埋め込まれた画像をこの方法で拡大表示すると、@<img>{low-resolution}のように文字はきれいだけど画像は荒いことが分かります。

//image[low-resolution][PDFに埋め込んだ画像は、文字と比べて解像度が荒いことが分かる]{
//}

ここで「画像の解像度が低い」という表現を使いましたが、正確には「画像のDPIが低い」といいます。DPI (Dot Per Inch)とは、1インチあたりのドット数を表します。これが低いと印刷したときに荒い画像になり、高いときれいな画像になります。

今までの方法で作った画像は、DPIが「72dpi」という低い値なので印刷には向きません。これを「300dpi」や「350dpi」ぐらいの高い値にすると、印刷してもきれいな画像になります。そのため、画像のDPIを「72dpi」からより高い値に変更する必要があります@<fn>{dpi1}@<fn>{dpi2}。
//footnote[dpi1][印刷用であれば、最低でも300dpiが必要とされています。場合によっては、450dpiや600dpiを使うこともあるようです。今回は、72dpiのちょうど5倍である360dpiを使うことにします。]
//footnote[dpi2][画像のDPIを変更しても、画像の大きさは変わりません。たとえば大きさが640x480である画像のDPIを72dpiから360dpiに変更しても、画像の大きさは640x480のままです。]

画像のDPIを変更するには、3つの方法があります。

1つ目は、Preview.appで変更する方法です。画像をPreview.appで表示し、メニューから「ツール→サイズを調整...」を選びます。ダイアログが表示され、解像度が「72ピクセル/インチ」になっているので、「360ピクセル/インチ」に変更し、「OK」ボタンを押します。

2つ目は、macOS付属の「sips」コマンドを使う方法です。Termnail.appを開き、Re:VIEWの原稿があるディレクトリに移動し、次のようなコマンドを実行します。

//cmd{
### sipsコマンドが存在することを確認
$ which sips
/usr/bin/sips

### 画像ファイルが存在することを確認
$ ls images/example1.png
images/example1.png

### 現在のDPIが72dpiであることを確認
$ sips -g all images/example1.png | grep dpi
  dpiWidth: 72.000
  dpiHeight: 72.000

### 360dpiに変更
$ sips -s dpiHeight 360 -s dpiWidth 360 images/example1.png
### または
$ mkdir images_360dpi
$ sips -s dpiHeight 360 -s dpiWidth 360 \
    --out images_360dpi/example1.png images/example1.png

### DPIが変更されたことを確認
$ sips -g all images/example1.png | grep dpi
  dpiWidth: 360.000
  dpiHeight: 360.000
//}

3つ目の方法は、ImageMagickの「@<code>{convert}」コマンドを使う方法です。brewやMacPortsでImageMagickをインストールし、次のようなコマンドを実行します。

//cmd{
### DPIを360dpiに変更
$ convert -density 360 -units PixelsPerInch images/example1.{png,png}

### DPIが 360dpi ÷ 2.54inch/cm ＝ 141.73 に変更されたことを確認
$ identify -verbose images/example1.png | egrep '(Resolution|Unit)'
  Resolution: 141.73x141.73
  Units: PixelsPerCentimeter
//}

これらの方法によって解像度を360dpiに変更した画像を埋め込んだのが、@<img>{example4}です。非常に小さくなってしまいました。

//image[example4][約束された勝利のサイクル(360dpi)]{
//}

この問題については次の章で説明します。


=={sec-image-enlarge} 画像を引き伸ばす

画像のDPIを大きくすると、PDFに埋め込んだら小さく表示されてしまいました。この問題を解決するには、画像の大きさを縦横5倍に引き伸ばすことです。具体的には次のような手順になります。

 1. Keynote.appでスライドを開き、ウィンドウ右上の「書類」アイコン→「書類」タブを選び、下のほうにある「スライドのサイズ」で「カスタムのスライドサイズ...」を選びます。
 2. デフォルトでは「幅：1024pt、高さ：768pt」になっているので、それぞれ5倍して「幅：5120pt、高さ：3840pt」にして、「OK」ボタンを押します（@<img>{keynote-enlarge}）。

//image[keynote-enlarge][スライドの大きさを5倍に広げる]{
//}

これでスライドの幅と高さが5倍になりました。それに伴って図形の大きさも5倍になったはずです。あとは、再度Command+AとCommand+Cを押し、Preview.appに切り替えてCommand+NとCommand+Sを押して画像を保存すると、幅と高さが5倍に引き伸ばされた画像ができます。またメニューから「ツール→サイズを調整...」を選んで、解像度を「360ピクセル/インチ」に変更してください@<fn>{dpi3}。
//footnote[dpi3][実は画像の大きさが十分大きければ、72dpiでもきれいに印刷されます。しかし余計なトラブルを回避するためにも、高いDPIに変更しておくのがいいでしょう。]

実際に原稿に埋め込むと、@<img>{example5}のようになりました。画像が小さくなっていないことが分かります。

//image[example5][約束された勝利のサイクル（幅と高さが5倍）]{
//}

またPDFファイルを拡大表示（Commandキーを押しながら「+」キーを押す）したのが、@<img>{high-resolution}です。以前のような画像の荒さが消えて、印刷に耐えうる解像度になっていることが分かります。

//image[high-resolution][PDFに埋め込んだ画像の荒さが消え、印刷に耐える解像度になっている]{
//}


=={sec-image-toggle} コンパイル速度の低下を防ぐ

画像を高解像度にすると、原稿をPDFへコンパイルするのがかなり遅くなります。正確にいうと、LaTeXファイルのコンパイル自体は変わりませんが、DVIファイル@<fn>{dvi}がらPDFファイルへの変換に時間がかかるようになります。特に原稿の締め切りが近いときは、大きなストレスになります。
//footnote[dvi][DVI (Device Independent) ファイルとは、デバイスに依存しない形式で表示情報を格納したファイルです。LaTeXファイルをコンパイルすると作成されます。このファイルをもとにPDFファイルが生成されます。]

この問題を解決するには、低解像度と高解像度の両方の画像を用意するのがいいでしょう。

 * 原稿執筆中は低解像度（72dpi）の画像を使ってコンパイルし、作業時間を短縮します。
 * 原稿が完成したら高解像度（360dpi）の画像を使ってコンパイルし、印刷用のPDFを生成します。
 * 低解像度用と高解像度用のフォルダを用意し、シンボリックリンクで切り替えます。

これだけ見ると簡単そうですが、準備は意外とかかります。以下に手順を示します。
@<br>{}

//noindent
【Step1】低解像度用と高解像度用のフォルダを作成します。imagesフォルダを消して、かわりにシンボリックリンクを作ります。

//cmd{
### 原稿のあるフォルダに移動し、imagesフォルダを退避
$ cd ~/work/mybook
$ ls -dF images
images/
$ mkdir old
$ mv images old

### 低解像度用(low-resolution)と高解像度用(high-resolution)のフォルダを作成
$ mkdir images_lowres
$ mkdir images_highres

### 低解像度用フォルダへのシンボリックリンクを作成する
### （原稿執筆中は低解像度用を使うため）
$ ln -s images_lowres images
//}

//noindent
【Step2】高解像度用の画像から低解像度の画像を生成するタスクを、Rakefileに追加します。@<list>{rake-images-task}のRubyコードを、Rakefileのいちばん最後に追加してください。

//list[rake-images-task][高解像度用の画像から低解像度の画像を生成するRakeタスク][ruby]{
desc "convert images"
task :images do
  ## macOSならsipsコマンドを使い、それ以外ではImageMagickを使う
  has_sips = File.exist?("/usr/bin/sips")
  ## 高解像度の画像をもとに低解像度の画像を作成する
  for src in Dir.glob("images_highres/**/*.{png,jpg,jpeg}")
    ## 低解像度の画像を作成済みなら残りの処理をスキップ
    dest = src.sub("images_highres/", "images_lowres/")
    next if File.exist?(dest) && File.mtime(src) == File.mtime(dest)
    ## 必要ならフォルダを作成
    dir = File.dirname(dest)
    mkdir_p dir if ! File.directory?(dir)
    ## 高解像度の画像のDPIを変更（72dpi→360dpi）
    if has_sips
      sh "sips -s dpiHeight 360 -s dpiWidth 360 #{src}"
    else
      sh "convert -density 360 -units PixelsPerInch #{src} #{src}"
    end
    ## 低解像度の画像を作成（72dpi、横幅1/5）
    if has_sips
      `sips -g pixelWidth #{src}` =~ /pixelWidth: (\d+)/
      option = "-s dpiHeight 72 -s dpiWidth 72 --resampleWidth #{$1.to_i / 5}"
      sh "sips #{option} --out #{dest} #{src}"
    else
      sh "convert -density 72 -units PixelsPerInch -resize 20% #{src} #{dest}"
    end
    ## 低解像度の画像のタイムスタンプを、高解像度の画像と同じにする
    ## （＝画像のタイムスタンプが違ったら、画像が更新されたと見なす）
    File.utime(File.atime(dest), File.mtime(src), dest)
  end
  ## 高解像度の画像が消されたら、低解像度の画像も消す
  for dest in Dir.glob("images_lowres/**/*").sort().reverse()
    src = dest.sub("images_lowres/", "images_highres/")
    rm_r dest if ! File.exist?(src)
  end
end
//}

//noindent
【Step3】高解像度用の画像を作成します。ポイントは次の通りです。

 * 縦横を5倍に引き伸ばしたスライドを作り、作図します。やり方は@<hd>{sec-image-enlarge}を参照してください。
 * 作図した図形の最背面に、幅4498ピクセル（＝ 900 * 5 - 2）の透明な四角形を埋め込みます。やり方は@<hd>{sec-image-width}を参照してください。
 * PNG画像に変換し、高解像度用のフォルダ（「images_highres」）に保存します。やり方は@<hd>{sec-image-save}を参照してください。このとき、画像の幅は2ピクセル追加されて4500ピクセルになっているはずです。

//noindent
【Step4】Terminal.appで「@<code>{rake images}」を実行して、高解像度の画像から低解像度の画像を自動生成します。そのあと「@<code>{rake pdf}」を実行すると、低解像度の画像を使ってPDFファイルが生成されます。

//cmd{
### 高解像度の画像から低解像度の画像を自動生成
$ rake images
### 低解像度の画像を使ってPDFファイルを生成
$ rake pdf
//}

//noindent
【Step5】原稿が書き終わったら、Terminal.appで「@<code>{ln -sfn images_highres images}」を実行して、高解像度の画像を使うようシンボリックリンクを切り替えます。そのあと「@<code>{rake pdf}」を実行すると、高解像度の画像を使ってコンパイルされます。もう一度低解像度に切り替えるには、「@<code>{ln -sfn images_lowres images}」を実行します。

//cmd{
### 高解像度の画像を使うよう切り替える
$ ln -sfn images_highres images
### 低解像度の画像を使って印刷用のPDFファイルを生成
$ rake clean
$ rake pdf
### もう一度低解像度に切り替える
$ ln -sfn images_lowres images
//}

また@<list>{rake-toggle-task}のRubyコードをRakefileの末尾に追加すると、「@<code>{rake images:toggle}」だけで低解像度と高解像度を切り替えできるようになります。UNIXコマンドに不慣れな人はこちらを使うほうがいいでしょう。

//list[rake-toggle-task][低解像度と高解像度を切り替えるRakeタスク][ruby]{
namespace "images" do

  desc "toggle directories ('images_{lowres,highres}')"
  task :toggle do
    link = File.readlink("images")
    rm "images"
    if link == "images_lowres"
      ln_s "images_highres", "images"
    else
      ln_s "images_lowres", "images"
    end
  end

end
//}

それでは、高解像度と低解像度の画像を切り替えると、コンパイル時間はどのくらい変わるでしょうか。画像を40枚以上含むような、とある同人誌で試してみました。

//cmd{
### 低解像度の場合
$ ln -s images_lowres images
$ time rake pdf
...(snip)...
real	0m12.029s
user	0m10.600s
sys	0m0.692s

### 高解像度の場合
$ ln -s images_highres images
$ time rake pdf
...(snip)...
real	1m23.288s
user	1m17.991s
sys	0m3.838s
//}

低解像度だと約12秒、高解像だと約83秒かかりました。比率で7倍、秒数で71秒の差です。これだけ差があるなら、手間をかける価値は十分あるでしょう。

なおここで説明した方法は、「高解像度と低解像度のどちらの画像をPDFに埋め込んでも幅と高さが同じになる」ことが前提条件です。そのためには、画像の解像度(DPI)が5倍なら縦と横の大きさも5倍にする必要があります。@<list>{rake-images-task}のRakeタスクはそのように考慮されています。


=={sec-image-misc} その他

 * 高解像度の画像を使った印刷用のPDFは、ファイルサイズが大きくなり、PDFビューワで読むときにスクロールがひっかかるようになります。そのため、ダウンロード用のPDFでは中解像度（144dpiや216dpi）の画像を使うといいでしょう。
 * ダウンロード用のPDFやePubでは、画像を圧縮しましょう。ファイルサイズが大きく減らせます。画像を圧縮するには、macOS用なら「ImageOptim.app」がお勧めです。
 * macOSで「Command+Shift+4 → スペースキー → 対象のウィンドウをクリック」という操作をすると、ウィンドウのスクリーンショットをシャドウつきで撮れます。このとき、対象のウィンドウをOptionキーを押しながらクリックすると、シャドウなしになります@<fn>{shadowless}。シャドウなしのほうが画像サイズを減らせるので、スクリーンショット画像が多い場合はシャドウなしも検討してみましょう。
 * スクリーンショットを撮るなら、Retinaディスプレイの機種で撮ると解像度が高くなるのでお勧めです。たとえば、Retinaでない機種で撮ったスクリーンショットが72dpiで640x480の大きさだとすると、同じスクリーンショットをRetinaディスプレイの機種で撮ると72dpiで1280x960の大きさになります。DPIは同じですが縦と横がそれぞれ2倍の大きさになることがわかります。あとはこれを@<hd>{sec-image-dpi}で紹介した方法で144dpiに変更すれば、画像の表示サイズは同じままでより解像度が高くなるので、印刷するときれいに見えます。サンプルとして、Retina機種で撮ったスクリーンショットを@<img>{AboutThisMacbook}に、Retinaでない機種で撮ったスクリーンショットを@<img>{AboutThisMacbookAir}に載せておきますので、拡大表示して違いを見てみてください。
//footnote[shadowless][ウィンドウスクリーンショットのシャドウを、なくすのではなく小さくする方法を探しましたが、見つかりませんでした。ご存知の方がいたらぜひ教えてください。]

//image[AboutThisMacbook][Retinaディスプレイの機種で撮ったスクリーンショット]{
//}
//image[AboutThisMacbookAir][Retinaでない機種で撮ったスクリーンショット]{
//}

=={sec-image-conclusion} まとめ

Keynote.appで描いた図を画像に変換してPDFに埋め込む方法を紹介しました。また印刷用に高解像度の画像を用意する方法と、低解像度と高解像度の画像を切り替える方法も紹介しました。他にもっとよいやり方があれば、皆さんぜひ公開してください。

====[column] Keynoteは最高の作図ツールである : @erukiti

これは僕の持論なのですが、エンジニアが作図するときに一番バランスのいいツールがKeynoteだと思います。機能が多すぎずちょうどいい程度にそろっているからです。Windowsの場合はPowerPointという手もあります。

筆者（@erukiti）は表紙、サークルカット、ポップ、値段表、さまざまなところでKeynoteを活用しています。

====[/column]
