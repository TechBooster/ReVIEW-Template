
= サイトを高速化した話

こんにちは、aumoに2022年に新卒入社したエンジニアのクリモトです。この章ではaumoの比較サイト@<fn>{comparison-site}表示速度改善の取り組みについて紹介します。

== サイトの表示が速いとどんなメリットがあるのか？
サイトの読み込みが速いと以下のようなメリットがあるとGoogleが報告しています@<fn>{business-impact}@<fn>{search-ranking}。

 * 広告などの売り上げが伸びる
 * ページの表示順位が上がる

このようにサイトの速度を改善することで、ビジネスに大きなインパクトを与えることができます。

また、サイトのレスポンス速度（≒表示速度）を改善することで、

 * 1リクエストに使われるサーバーのリソースが減り、サーバーの台数を減らせる
 * デバッグ時の待ち時間が減り、開発効率が上がる

といった効果も見込めます。

それでは、この重要なサイト表示速度改善のためにaumoで実際に行った施策を見ていきましょう！

== バックエンドのレスポンス速度を改善する
aumoの比較サイトは、Rails × GraphQLを用いたバックエンドとNuxt.jsが動作するフロントエンドから構成されています。今回はまずバックエンドの改善から取り組みました。

=== まずはバックエンドサーバのレスポンス速度の推移をグラフ化する
今回の速度改善プロジェクトが始まった時点では、aumoの比較サイトのバックエンドにはレスポンス速度を収集する基盤が特にありませんでした。この状態でプロジェクトを進めても、施策の効果が分からないので、まずは

 1. サーバーのアクセスログを収集
 2. アクセスログを集計し、日毎のレスポンス速度の変化をグラフ化する　

ことから始めました。

アクセスログの集計には、「DatadogのようなAPMサービスを使う」か「NGINXのログを、fluentdで吸い上げてBigQueryにアップロードする」の2つが候補として上がりました。最終的には

 * BigQueryの方がAPMサービスより安く済みそう
 * APMサービスを用いてGraphQLのスキーマごとにメトリクスを取る方法がネット上にあまり無い
 * Nginx × Fluentd × BigQueryの構成はaumoで既に使ったことがあり、その際の知識が活かせる

ということで、「NGINXのログを、Fluentdで吸い上げてBigQueryにアップロードする」方法を採用しました。そして最後にBigQueryのデータをData Studioに取り込みグラフ化することで、以下のようにGraphQLのスキーマごとに日々のレスポンス速度の変化が一目でわかるようになりました。

//image[server_response_before][レスポンス速度推移]{
//}

Data Studioを自分は今回初めて使ったのですが、無料で表現力のあるグラフを簡単に作成できて凄いですね。簡単なメトリクスを計測したいだけなら、DataDogのようなAPMサービスは不要な気がします。

=== N+1を解消する
メトリクスが取れるようになったので、ここからは遅いクエリを1つ1つ潰していきます。GraphQLでN＋1が大量に起きていることが、僕が入社する前から問題になっていたらしいので、まずは実際に1リクエストでどれくらいのクエリが発行されているのかを確認してみました(railsのSQLのボトルネックを探すには、rack-mini-profiler@<fn>{rack-mini-profiler}が便利です)。その結果が↓

//image[sql_count_before][N+1解消前]{
//}

まさかの1リクエストに1827個のクエリが発行されているという驚愕の結果に（予想より桁が1つ多かった笑）

N＋1の対策としては、rack-mini-profilerを使うと、クエリがコードのどの部分で発行されているかが分かるので、N+1が発生している箇所を一つずつ地道に特定しては、ActiveRecordのincludesメソッドを使ってEager Loadingをする方法を取りました。その結果1リクエストあたりのクエリ発行数を52個まで削減することができました！

//image[sql_count_after][N+1解消後]{
//}

ただ以下のような多対多の関係にあるモデルをincludesすると、spotsテーブルからspot_photosテーブルを経由してphotosテーブルのレコードを1つだけ取得したい場合でも、spot_photosテーブルのインスタンスが、spotに紐づくphotoの数だけ生成され、紐づくphotoの数が増えれば増えるほどレスポンスが遅くなっていく問題が発生することが分かりました。N+1が発生していた時の方がむしろ高速に動作する事態になったため、一部のテーブルではあえてN+1を許容する戦略を取ることにしました（本当は非正規化されたテーブルを作成したりした方が良いのですが、そこまで劇的な速度改善は見込めなかったので、その方法は採用しませんでした）。

//image[intermediate_table][多対多の関係]{
//}

=== MySQLをElasticsearchに移行
N+1は解消されたものの、joinとsortが組み合わされていて1クエリに5秒程度かかってしまう以下のようなクエリがありました。

//source{
SELECT
 articles.*
FROM
 articles
INNER JOIN
 article_keywords ON article_keywords.article_id = articles.id
WHERE
 articles.platform IN (0, 1)
 AND article_keywords.keyword_id = 1
ORDER BY
 articles.published_at DESC
LIMIT 6
//}

こういったクエリは通常は非正規化したテーブルを作成して対処したりすると思うのですが、aumoでは全文検索エンジンとしてElasticsearchを使っており、ariticleのインデックスも既に作成済みだったので、Elasticsearchを非正規化されたテーブルの代わりに使用することにしました。

その結果5秒ほどかかっていたクエリを10ミリ秒まで短縮することができました。

=== サーバー台数を追加
これらの施策を全てやっても依然としてレスポンスがたまに遅くなることがありました。BigQueryに溜まっているログをレスポンスが遅い順にsortしてみると、遅いリクエストが来た時刻が固まっており、一時的に想定よりも多いアクセスがきてしまった結果、レスポンス速度が悪化しているようでした。そこでサーバーの台数を増設する対策をとることで、レスポンス速度が常に安定するようになりました。

=== バックエンドのチューニング結果
以上のサーバーサイド周りの改善を全てやった結果、今回修正を入れたGraphQLのクエリはどれも最初の1/2程度までレスポンスにかかる時間を減らすことができました！

//image[server_response_after][サーバーレスポンスの推移]{
//}

またN+1が解消されDBへの負荷が減った影響で、DBインスタンスを一台減らすこともでき、インフラ費用が削減されました。

//image[slack_comment][slackでのコメント]{
//}

== フロントエンド周りのチューニング
サーバーサイドの改善は一通り終わったので、次はフロントエンドの改善を行っていきます。フロントエンドのチューニングはGoogleが提供しているLighthouse@<fn>{light-house}が改善項目を色々と教えてくれるので、それを愚直に潰していく方針で進めました。

=== 不要なjavascriptを削減する
Lighthouseに「不要なJavaScriptがある」と警告が出ていたので、webpack-bundle-analyzer@<fn>{webpack-bundle-analyzer}を使用して不要なJavaScriptが無いか見てみました。

//image[moment_before][webpack-bundle-analyzerの結果]{
//}

すると、momentのlocaleが日本語以外の不要なlocaleまで取り込んでしまっており、これを改善するのが簡単に大きな効果を得られそうだったので、日本語のlocaleファイルのみを読み込むように変更しました。その結果localeの圧縮後のファイルサイズを47KB -> 701B まで減らすことができました。

本当はmomentを自作の関数に置き換えてmoduleごと削除したかったのですが、momentがコードのいたる部分で使われており、それなりに時間がかかりそうだったので、今回はやりませんでした。他にもswiper@<fn>{swiper}というカルーセル用のmoduleもCSSで置き換えることで、かなりの量のファイルサイズを軽減出来そうだったのですが、こちらも時間がかかりそうだったので着手しませんでした。今後はこれらのmoduleは使わないようにしつつ、時間があるときに少しずつ置き換えていけると良いなと考えています。

その他にもwebpackのchunckの設定を変更するといったチューニングを行い、読み込まれるJavaScriptのファイルサイズを削減しました。

=== CSS、JavaScript、Imageの遅延読み込み
続いてLighthouseにCSS、JavaScript、Imageの遅延読み込みを提案されていたので、重要でないJavaScriptとCSSは全て遅延読み込みをさせるようにしました。

//image[lazyload_js][Lighthouseの警告1]{
//}

//image[lazyload_image][Lighthouseの警告2]{
//}

なお、画像を遅延読み込みする場合は、予め画像が表示される部分の領域を確保しておかないと、Lighthouseの指標の1つであるCLS(表示されるページコンテンツにおける予期しないレイアウトのずれ)が悪化するので注意が必要です。

=== サーバー台数を増やす
サーバーサイドと同じく、フロンエンドのサーバー台数も、リクエスト数に追い付いていない時間帯がありました。

原因としては、aumoの比較サイトではフロントエンドにNuxt.jsを採用しており、Nuxt.jsの基盤にあるNode.jsは、シングルプロセス・シングルスレッドで動いています。しかし、aumoではCPUコア数が多いインスタンスタイプを少ない台数で稼働させており、リソースを上手く使い切れていませんでした。そこで、コア数の少ないインスタンスタイプを多数動かす構成に変えることで、サーバー費用をそれ程増やさずにより多くのリクエストを捌けるようにしました。

成長過程にあるサービスを運営している場合は、リクエスト数も時間が経つごとにどんどん増えていくので定期的にサーバー台数やスペックを見直していく必要があるということを学べて良かったです。

=== フロントエンドのチューニング結果
速度改善プロジェクト前とプロジェクト後で比較すると、Lighthouseの指標を大きく改善することが出来ました。

//image[light_house][チューニングの結果]{
//}

また、今回のプロジェクト前は比較サイトの開発をする時は、コードの変更を確認するためにページをリロードすると30秒近く待たされて、非常に開発効率が悪かったのですが、今では待ち時間も10秒程度まで短縮され、開発効率もかなり良くなりました。

== まとめ
今回の速度改善プロジェクトの結果、無事サイトの表示速度が改善され、インフラ費用を削減することもできました。サイトの速度が改善されたことでページの表示順位にもプラスの効果が現れることも期待はしていたのですが、現時点では、そのような効果は現れていません。Googleのアルゴリズムがサイトの表示順位を決める際に一番重視するのは、やはりサイトのコンテンツであり、表示速度はそこまで重視はしていないということなのでしょう。

//footnote[business-impact][https://developers-jp.googleblog.com/2021/05/core-web-vitals.html]
//footnote[search-ranking][https://developers.google.com/search/blog/2018/01/using-page-speed-in-mobile-search?hl=ja]
//footnote[webpack-bundle-analyzer][https://github.com/webpack-contrib/webpack-bundle-analyzer]
//footnote[light-house][https://chrome.google.com/webstore/detail/lighthouse/blipmdconlkpinefehnmjammfjpmpbjk?hl=ja]
//footnote[comparison-site][https://gourmet.aumo.jp/]
//footnote[rack-mini-profiler][https://github.com/MiniProfiler/rack-mini-profiler]
//footnote[swiper][https://www.npmjs.com/package/swiper]
