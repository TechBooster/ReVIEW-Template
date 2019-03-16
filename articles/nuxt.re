= NuxtでAIアプリのフロントを作ろう
それでは、今まで@hidefに設計してもらったAIアプリを実際に実装していきましょう。

今回のサンプルアプリケーションはすべて次のGitHubリポジトリにあります。
本文の説明と合わせて参照いただけると理解が進みます。
#@# Githubリンクを入れる

== 今回作るアプリ
今回は、次のような構成でアプリを実装していきます。
#@# アプリの構成図を入れる
#@# アプリの構成の説明

本アプリは主に、フロントをNuxt.jsというフロントエンドフレームワークを用いて
実装していきます。
本章では、そのフロント部の実装を中心に説明していきます。

== Nuxt.jsとは
Nuxt.jsとは
#@# 説明追加

=== Nuxt.jsを使うメリット
#@# 説明追加

== 実際にプロジェクトを作ってみよう
ここからは、実際に手順を追ってNuxt.jsを使ってフロントを組み立ていきます。
なお、Nuxt.jsを動作させるためにはNode.jsの動作環境が必要です。

Node.jsのローカル動作環境のインストールについて、今回は省略しますので、適宜、Web等で調査いただき
導入をお願いします。

途中で、後章の本番デプロイや開発の生産性を上げるためにDockerを導入する手順があります。
最初からDockerを用いて、プロジェクト作成等を行うのは個人的にあまり好きでないので、最初のプロジェクト作成時はローカル環境を導入して
作成を行うことをお勧めします。

=== 今回作成するプロジェクトのバージョン
#@# プロジェクトのバージョン追加


=== プロジェクト作成
まずは、プロジェクトを作成していきましょう。
今回は、create-nuxt-appというツールを用いて、Nuxtのプロジェクトを作成します。

create-nuxt-appはコマンド一発でNuxtの必要なモジュールを取得して、プロジェクト・ディレクトリを作成してくれ非常に便利です。

他にもvue-cliなどNuxtのプロジェクトを作成する方法がありますが、Nuxtでプロジェクトを作る場合は、2019年現在 create-nuxt-appを利用するのが非常にお勧めです。

//cmd{
// プロジェクトの配下に移動した上で
npx create-nuxt-app
//}

次のような形にダイアログが出てくるので回答していきます。

//emlist{
npx: installed 379 in 17.006s
> Generating Nuxt.js project in C:\dev\NullSuck
? Project name NullSuck
? Project description My splendid Nuxt.js project
? Use a custom server framework none
? Choose features to install Progressive Web App (PWA) Support, Linter / Formatter, Prettier, Axios
? Use a custom UI framework vuetify
? Use a custom test framework none
? Choose rendering mode Universal
? Author name pco2699
? Choose a package manager yarn
//}

これだけで、とりあえずプロジェクト構成ができてしまいます。

=== プロジェクトのTypeScript対応を行う
2019年2月27日現在、create-nuxt-appで作成したプロジェクトはTypeScriptに対応していません。
著者の個人的な好みによりTypeScriptに置き換えて動かします。

やることは次のとおりです。
 * package.json
 ** コマンド部分のnuxt -> nuxt-ts に書き換え
 ** 不要なeslintパッケージを削除
 * nuxt.config.js
 ** 拡張子をjs -> tsに変更
 ** Build Configulationのeslint-loaderをtslint-loaderに書き換える
 ** Build Configulationの引数にany型を付与する
 * tsconfig.json/tslint.jsonを作成する

#@# 具体的なソースは貼っつけたほうが良さそう
これらをやったら次のコマンドで立ち上げてみましょう。

//cmd{
$ yarn run dev
//}

ブラウザを起動し、localhost:3000にアクセス。次のような画面が立ち上がったら、無事に起動できています。
おめでとうございます。

=== メインページの構成をVuetifyで作ろう
次に、デザインをもとにメインページの構成を作っていきましょう。
なお、デザインは深野氏によってAdobe XDで作成されています。

こちらが、メインページの構成です。

=== ヌルサクグラフを実装しよう



