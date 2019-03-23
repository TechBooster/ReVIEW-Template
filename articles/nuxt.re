={nuxt} NuxtでAIアプリのフロントを作ろう
それでは、今まで@hidefknに設計してもらったAIアプリを実際に実装していきましょう。

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

=== 全体のプロジェクト構成
全体のプロジェクト構成は次の通りです。

//emlist[whole_project]{
NullSuck-AI            
├── client
├── server             
├── docker-compose.yml 
├── k8s                
├── k8s.yml            
├── LICENSE            
└── makefile           
                       
5 directories, 5 files 
//}

==== client
本章で説明するNuxt.jsを用いたフロントエンドのソースコードを格納しています。

==== server
@<chapref>{nuxt} で説明するPythonを用いたバックエンドのソースコードを格納しています。

==== その他
@<chapref>{deploy} で説明するデプロイに用いるファイルなどを格納しています。

== Nuxt.jsとは
Nuxt.jsとは、Vue.jsアプリケーションをつくるためのフレームワークです。
Vue.jsもフレームワークなので、フレームワークをつくるためをフレームワーク、ということになります。

=== Vue.js
まず、Vue.jsから説明すると、Vue.jsはフロントエンドのフレームワークで、JavaScriptのフレームワークです。
いままで、JavaScript/HTML5/CSS3で、フロントエンドでガリガリ書こうとすると、レイアウトとコードが混在してしまうため
どうしてもコードが汚くなったり、複雑な動きの実装が難しくなってしまいます。

そこで、出てきたのがフロントエンドフレームワークです。近年だと AngularJS/React/Vue.jsが有名です。
Vue.jsはその中でも、HTML5/CSS3などの知識を活かせて学習コストが低いフレームワークとして近年人気が出ています。

=== Nuxt.js
そして、そのVue.jsのフレームワークとして出てきたのがNuxt.jsが有名です。
Vue.jsそのものは単純にビューのフレームワークであるため、環境構築の仕方や使い方などは実は非常に多様でした。

* jQueryと同じようにHTMLにincludeして使う
* npmを用いて導入する

その他、さまざまな導入方法があります。
これだと、初学者はもちろん、そこそこVue.jsに強いひとでも自分なりの方法で環境構築や開発のしやすい
プロジェクト構成を考える必要があり、とっつきづらい原因となっていました。

そこで、Nuxt.jsは、そういった環境やミドルウェアをまとめてフレームワークとして扱うことによって
開発のしやすさを向上させたフレームワークです。

具体的には、以下のものがパッケージされています。

 * Vue 2
 * Vue-Router
 * Vuex
 * Vue Server Renderer
 * vue-meta
 * webpack
 * babel

=== Nuxt.jsを使うメリット
Nuxt.jsと調べるとよく「サーバサイドレンダリングできるフレームワーク」とサーバサイドレンダリングの一面が強調されます。
しかし、個人的には、上記、述べたとおりVue.jsに必要なミドルウェアやプロジェクト構成がすべて用意されている、というのがNuxt.jsのメリットであると考えています。

例えば、Nuxt.jsを使わずに、Vue.jsを使おうとするとWebpackというフロントエンドで使われるライブラリの知識が必ず必要になります。@<fn>{webpack}

Nuxt.jsを利用すると、このWebpackなどを含めて多くがそもそも含まれていたり、プラグインという形で提供されており
開発が非常にしやすいのがNuxt.jsを使うメリットです。

== 実際にプロジェクトを作ってみよう
ここからは、実際に手順を追ってNuxt.jsを使ってフロントを組み立てていきます。
なお、Nuxt.jsを動作させるためにはNode.jsの動作環境が必要です。

Node.jsのローカル動作環境のインストールについて、今回は省略しますので、適宜、Web等で調査いただき
導入をお願いします。

後章の本番デプロイにDockerを導入する手順があります。
最初からDockerを用いて、プロジェクト作成等を行うのは個人的にあまり好きでないので、最初のプロジェクト作成時はローカル環境を導入して
作成を行うことをお勧めします。

=== 今回作成するプロジェクトのバージョン
今回は次のバージョンで動作しています
//table[nuxt_ver][nuxt_ver]{
プロダクト名	バージョン
-----------------------
Nuxt.js	2.4.0
Node.js	10.14
Vuetify 1.5.0
//}


=== プロジェクト作成
まずは、プロジェクトを作成していきましょう。
今回は、@<tt>{create-nuxt-app} というツールを用いて、Nuxt.jsのプロジェクトを作成します。

create-nuxt-appはコマンド一発でNuxtの必要なモジュールを取得して、プロジェクト・ディレクトリを作成してくれて非常に便利です。

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
? Choose features to install Progressive Web App (PWA) Support,
                             Linter / Formatter,
                             Prettier,
                             Axios
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

本節は必須ではないので、興味がなければ読み飛ばしてしまってください。
2019年現在だと、NuxtはJavaScriptで書いたほうがサクッと作れます。

やることは次のとおりです。

今回はlinterとしてtslintを導入しています。 @<fn>{tslint}

 * package.json
 ** コマンド部分のnuxt -> nuxt-ts に書き換え
 ** 不要なeslintパッケージを削除
 * nuxt.config.js
 ** 拡張子をjs -> tsに変更
 ** Build Configulationのeslint-loaderをtslint-loaderに書き換える
 ** Build Configulationの引数にany型を付与する
 * tsconfig.json/tslint.jsonを作成する
 * 必要なパッケージの追加
 ** tslint-loader
 ** nuxt-class-component
 ** vue-property-decorator

上記の対応を行うと、次のようにデコレータを用いてVueファイルを作れます。非常にきもちいいです。

//list[typescript_vue_sample][TypeScriptでデコレータを用いたコンポーネント]{
  @Component
  export default class FormCard extends Vue  {
    @Mutation('SET_WINE_VALUE') setWineValue
    @Prop({type: Object}) wineAttr
    @Prop({type: Number}) total

    set(val: number){
      this.setWineValue({ id: this.wineAttr.id, value: val})
    }
  }
//}
 
=== プロジェクトの立ち上げ
#@# 具体的なソースは貼っつけたほうが良さそう
これらをやったら次のコマンドで立ち上げてみましょう。

//cmd{
$ yarn run dev
//}

ブラウザを起動し、localhost:3000にアクセス。次のような画面が立ち上がったら、無事に起動できています。
おめでとうございます。

//image[first_nuxt_screen][起動時の画面]

=== プロジェクト構成
今までの手順通りにプロジェクトを作成していくと、以下のような構成になるはずです。

//emlist{
client/                  
├── assets              
├── components ★                   
├── layouts ★           
├── middleware
├── pages ★                
├── plugins
├── static               
├── store ★                
├── node_modules         
├── nuxt.config.ts       
├── package.json         
├── yarn.lock            
├── tsconfig.json        
├── tslint.json          
└── README.md            
                         
9 directories, 8 files   
//}

この中で★をつけているディレクトリを今回、重点的にいじります。
それぞれの内容をします。

==== components
画面の中の一部品(コンポーネントと呼ぶ)を格納する場所です。

==== layouts
画面の共通構成を格納する場所です。

==== store
Vuexと呼ばれる状態管理のためのファイルを格納する場所です。

==== pages
画面の中の一ページを格納する場所です。
こちらに保存されたファイル名で、そのままURLのルーティングになります。

例えば、次のような配置とした場合

//emlist{
pages/
--| user/
-----| index.vue
-----| one.vue
--| index.vue
//}

次のようなルーティングになります。
(ドメイン名は @<tt>{https://ai.pco2699.net/} とします。)

//emlist{
https://ai.pco2699.net/ -> index.vue
                      /user/ -> user/index.vue
                      /user/one -> user/one.vue
//}

== アプリのデザイン構成をVuetifyで作ろう
まずは、デザインをもとにアプリの基礎構成を作っていきましょう。
デザインは既にhidefkn氏に説明いただいた通りです。

この節は、まずVueの単一ファイルコンポーネントの基本構成について説明しつつ
次のような基礎構成をつくることを目標にします。

//image[first_layout][今回の目標]

今回は、@<tt>{layouts/default.vue}をいじっていきます。

default.vueにズラズラと既にテンプレートのコードが書かれていると思いますが
@<b>{めんどくさいので一旦、すべて削除します（笑）}

そして次の形で、初期構成を作りましょう
//list[first_code_nuxt][default.vue]{
<template>
  <v-app>
    <!-- この中にHTMLを記載する -->
  </v-app>
</template>

<script lang="ts">
  // この中にTypeScriptを記載する
  import Vue from 'Vue'
  import Component from 'nuxt-class-component'

  @Component
  export default class Layout extends Vue {}
</script>

<style scoped>
  /* この中にCSSを記載してstyleを定義 */
</style>
//}
上記の形がVueの単一ファイルコンポーネントの基本形です。
@<code>{template}, @<code>{style}, @<code>{script} は順番はどの順番でもよいですし、
不要であれば、そもそも消してしまって@<code>{template}だけの記載でもOKです。

今回は大体の部分をVuetifyのUIパーツを利用するため@<code>{style}はほとんど利用しません。

=== テンプレート部をつくる
では、まず上記の中で@<code>{template}、つまりHTML部を作っていきましょう。
Vuetifyは決められたコンポーネントを組み合わせていくことで、簡単にきれいなページを作れるライブラリです。

どのようなコンポーネントがあるかはVuetifyの公式ホームページ（@<href>{https://vuetifyjs.com}）にあるので、参照してみてください。

今回は次の図に従って、コンポーネントを記載していきます。
#@# Githubリンクを入れる


== 診断画面をつくろう
=== 診断項目カードをつくろう




== ヌルサクグラフを実装しよう
機械学習やデータサイエンスではおなじみ、データの可視化をNuxt.jsで実装してみましょう。
次に示す診断画面のグラフ部分の実装を説明します。

=== 今回 利用するグラフライブラリ
JavaScript用やフロントエンド用のグラフフレームワークはいろいろあります。

 * Chart.js
 * D3.js
 * HighCharts.js

一番メジャーなのは Chart.jsだと思いますが、今回は amCharts（@<href>{https://www.amcharts.com/}）を利用します。
理由は、次の通りです。

 * 公式ページからグラフのタイプや見た目を検索でき、いいと思ったやつはコードが書かれているので@<b>{そのままコピーすれば実装できる}
 * 基本、有料ではあるが、ロゴを表示すれば無料で利用可
 * グラフの見た目がヌルサク

 早速、

== APIに投げるためにVuexを実装しよう


//footnote[webpack][もちろん、パフォーマンスチューニングや本格的なパッケージングを行う場合は、Nuxt.jsでもwebpackの知識は必要になります。]
//footnote[tslint][TypeScriptのlintは、eslintというJS用のlinterをts用にコンバージトしたものか、ts用のtslintを利用、２つの方法があります。]
