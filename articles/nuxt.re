={nuxt} NuxtでAIアプリのフロントを作ろう
それでは、今まで@hidefknに設計してもらったAIアプリを実際に実装していきましょう。

今回のサンプルアプリケーションのコードはすべて @<href>{https://github.com/pco2699/NullSuck-AI} で見れます。
本文の説明と合わせて参照いただけると理解が進みます。

また、本アプリは @<href>{https://ai.pco2699.net} にデプロイされているので
興味がある方はアクセスしていじってみてください。

== 今回作るアプリ
今回は、次のような構成でアプリを実装していきます。
//image[project_stack][アプリの概観図]

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

//image[first_layout][今回の目標][scale=0.6]

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
//image[first_vuetify][Vuetifyを用いた画面の構成]

実際にコーディングすると次の形となります。
//list[vuetify][default.vue][html]{
<v-app>
  <v-toolbar
    app
    color="primary"
    class="white--text"
  >
    <v-toolbar-side-icon class="white--text"/>
    <v-toolbar-title v-text="title" />
  </v-toolbar>
  <v-content>
    <v-container>
      <nuxt/>
    </v-container>
  </v-content>
  <v-footer
    app
  >
    <span>&copy; Norwegian Geek 2019</span>
  </v-footer>
</v-app>
//{

toolbarの色を@<tt>{primary}で指定しています。
このprimaryの色は@<tt>{plugins/vuetify.js}に記載されている色を変更することで
変えることができます。

//list[primary_color][Vuetifyの色指定]{
Vue.use(Vuetify, {
  theme: {
    @<tt>{primary: colors.red.darken4},
    accent: colors.grey.darken3,
    (略)
  }
})
//}

テキストの色も @<tt>{class="white--text"}などのクラスを付与することで、色を白くできたりと
とにかくCSSを書かなくてもさまざまな装飾、レスポンシブ対応を行うことができます。

== 診断画面をつくろう
次に、ワインの項目を入力する診断画面を作ってみましょう。
診断画面は次に示すような画面です。

//image[question_screen][診断画面]

診断画面は質問数の分だけカードが並んでいます。
今回は合計で11コの質問があるので、11コカードが並んでいる形です。

これを普通にHTMLなどで書くとコピペの嵐となっていますが
Nuxt.js/Vue.jsだとプログラマブルに、冗長なく書けるのが嬉しいところです。

こういった繰り返しの部品は「コンポーネント」として作成します。
この節では、診断画面のカードをつくることでコンポーネントの作り方を学びます。

=== 診断項目カードをつくろう
診断項目カードなどのコンポーネントを作るには、@<tt>{components}フォルダの下に
vueファイルを作ります。

ディレクトリ @<tt>{components}の下に@<tt>{FormCard.vue} という名前でファイルを作りましょう。

FormCardも先程と同様に既にあるVuetifyのUIコンポーネントを組み合わせてつくることができます。

//list[][]{
<template>
  <v-card class="ma-2 pa-1">
    <div class="subheading">
      Heading
    </div>
    <div class="caption">
      1/11
    </div>
    <v-slider
      class="px-5 pt-5 pb-3"
      thumb-label="always"
      height="3"
      hide-details
    ></v-slider>
  </v-card>
</template>

<script lang="ts">
import Vue from 'vue'

export default class FormCard extends Vue {}
</script>
//}

ソースコードと画面の紐づけは次のような形となっています。
v-cardのclassに紐付いている@<code>{ma-2 pa-1}は、マージンとパディングの指定を表しています。
マージンやパディングもVuetifyを使うとclassで書いてしまうことができます。

=== 診断カードをたくさん出せるようにしよう
先程の診断カードを実際にページに出してみましょう。
ページに出すには、ディレクトリ @<tt>{pages}にある@<tt>{index.vue}に@<tt>{formcard.vue}を
インポートすることで、表示することができます。

実際に、インポートしてみましょう。
//list[index_vue][index.vue]{
<template>
  <v-layout column>
    (省略)
    <!-- HTMLタグの形式で埋め込める！ -->
    <!-- v-forでこのタグを繰り返し出力できる -->
    <form-card v-for="i in 11" :key="`${i}`"/>
    <v-btn @click="submit">診断結果を表示する</v-btn>
  </v-layout>
</template>
<script lang="ts">
  // importでさきほどのFormCard.vueを取り込み
  import FormCard from '~/components/Form/FormCard.vue'
  import Component from 'nuxt-class-component'

  // デコレータ内で、このコンポーネントを
  // 使うことを宣言
  @Component({
    components: {
      FormCard
    }})
  export default class Index extends Vue { }
</script>
//}

上記の通り書くことで、診断カードを11コ表示させることができます！
注意点は下のとおりです。

 * この書き方はTypeScipt/ES6のクラスベースになっています。
   他の本やページなどでは、別の書き方をしてるかもしれませんが
   それでももちろん動きます。
 * JS/TSの世界ではFormCardとキャメルケースですが
   HTMLのタグになると、form-cardとスネークケースになるので
   気をつけてください。


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

今回、使うグラフは、このページ()から持ってきます。

#@# ページの図を入れる
このグラフをFormCardと同じ要領でコンポーネント化してみましょう。ファイル名は@<code>{ResultChart.vue}としましょう。
グラフをコンポーネント化するには、amChartsにあるTypeScript/ES2015と書かれたコードをほぼそのまま使うことができます。

該当のコードを、次の通りそのままはっつけてしまいましょう。

//list[Result_chart][ResultChart.vue]{
  <template>
  <div>
    <div id="chartdiv"></div>
  </div>
</template>

<script lang="js">
  import Vue from 'vue'
  // import文をamChartsから持ってくる！
  import Component  from 'nuxt-class-component'
  import * as am4core from "@amcharts/amcharts4/core"
  (省略)

  am4core.useTheme(am4themes_animated);
  am4core.useTheme(am4themes_themes_dataviz);

  @Component
  export default class ResultChart extends Vue {
    result = 0

    mounted () {
      // import文以降をはっつけてしまう！

      // Create chart instance
      let chart = am4core.create("chartdiv", am4charts.RadarChart);
      const data = []
      const wineAttr = this.$store.getters.GET_WINE_ATTR
      for (let i in 12){
        data.push({category: i, value: i});
      }

      chart.data = data;
      chart.radius = am4core.percent(100);
      (省略)
    }
  }
</script>
//}

@<tt>{mounted}はコンポーネントが生成されたタイミングの中でもDOMが生成されたあとに走る処理です。
このタイミングで、amChartsのコードを走らせると、グラフが簡単に作成！、といった要領です。

今回のコンポーネントだけ、例外的にTypeScriptではなくJavaScriptで書いています。@<fb>{why_hs}

これでヌルサクグラフが完成です！

== APIに投げるためにVuexを実装しよう
今までのコンポーネントは見た目はそれっぽいですが、実際のデータが注入されていません。
そこで、実際のデータを注入しましょう。

今回は、Vuexを用いてデータを注入します。

表示に関するデータを、コンポーネントごとに管理するとめんどくさいよねー、それなら、一つの場所にまとめてしまおう！
というのがVuexの考え方だと思っています。

この一つの場所、というのがStoreおよびStateと呼ばれる場所になります。

=== Stateを定義しよう
では、Stateを実際に定義してみましょう。
Stateは、先ほど、説明したディレクトリの@<tt>{store}の配下にファイルをつくることで
Nuxt.jsがよしなに、コンポーネントへの注入などをやってくれます。

早速、@<tt>{store}の配下に、@<tt>{index.ts}を作ってみましょう。

実際に作った、index.tsのstateは次の通りです。

//list[indexts][index.ts][typescipt]{

// 入力値のワインの属性値などを保持する
// WineAttributeのインターフェース
export interface WineAttribute {
  id: number
  japanese_title: string
  english_title: string
  value: number
  step: number
  max_value: number
  min_value: number
}


// Stateのインターフェース
export interface State {
  title: string
  wine_attributes: WineAttribute[]
  result: Result
}

// フォームから入力した
// 値のインターフェース
export interface InputValue {
  id: number
  value: number
}

// 実際のState
const s = (): State => {
  return {
    title: '',
    wine_attributes: [],
    result: { value: 0, comment: '' }
  }
}

export s as state
//}

Stateは先程も言った通り、アプリ中すべてのコンポーネントから使われるデータなので
いろいろなデータを持つ必要があります。

@<b>{どんな値がはいってくるべきか}をTypeScriptのinterfaceで定義しているのが前半部です。
interfaceとは、型に近い概念で、どんな値が入ってくるべきかを定義できるものです。
JavaScriptだとintaerfaceが使えないため、stateにどんな値が入ってくるかわからないのですが
TypeScriptinterfaceで事前に定義しておくことができます。

これがNuxt.jsでTypeScriptを利用する一番のメリットではないか、と個人的に思っています。

=== stateを変更するMutations, 取得するGetters
stateは、すべてのコンポーネントから取得できるので、ほいほい色んな値を入れられてしまうと困ってしまいます。

そのため、stateをいじれるやつを決めておくようにしよう！ということで以下の２つの概念が登場します。

 * Stateを変更するMutations
 * Stateを取得するGetters

各コンポーネントはstateを直接変更するのではなく
stateを変更したいときはMutations、取得したい場合はGettersをコンポーネントから呼び出します。

実際の定義はこのようになっています。

//list[mut_get][Mutations/Getters][typescript]{
const mutations: MutationTree<State> = {
  // ワインの属性値をセットするMutationの関数
  SET_WINE_ATTR: (state, wineAttributes: WineAttribute[]) => {
    state.wine_attributes = wineAttributes
  },
  // 入力値をセットするMutation
  SET_WINE_VALUE: (state, payload: { id: number; value: number }) => {
    state.wine_attributes[payload.id - 1].value = payload.value
  },
}

const getters: GetterTree<State, any> = {
  // ワインの属性値を取得するGetters
  GET_WINE_ATTR: (state): WineAttribute[] => {
    return state.wine_attributes
  },
}
//}

=== APIにアクセスするActions

APIにアクセスしてなにか、値を取得したい！という場合はActionsを使います。
Actionsは非同期前提で、ActionsからMutationsを呼んで、stateを更新します。

今回、「アルコール濃度」などのワインの属性値はAPI側に持っているため
これをActionsで呼び出して取得しています。

//list[actions][Mutations/Getters][typescript]{
const actions: ActionTree<State, any> = {
  ワインの属性値を予めAPIから取得するAction関数
  async nuxtServerInit(store: ActionContext<State, any>) {
    const res = await (this as any).$axios.$get('/api/wine_attributes')
      if (res.wine_attributes) {
        store.commit('SET_WINE_ATTR', res.wine_attributes)
      }
    }
  }
}
//}

nuxtServerInitという名前でActionsを定義しておくと
事前にサーバ側でAPIからデータを取得しておいて、画面に描画しておいてくれます。


=== コンポーネントにVuexを組み込もう
今まで説明してきたVuexの関数を実際にコンポーネントに組み込んでみましょう。

さきほど、つくった診断画面にワインの属性値を設定してみます。

//list[vue_corp][コンポーネントにVuexを組み込む]{
<template>
  <v-layout column>
    // form-cardを書き換える
    // これでワインの属性値を取得してform-cardに
    // 設定できる
    <form-card v-for="wineAttr in wineAttributes" 
               :key="`${wineAttr.id}`"
               :wine-attr="wineAttr"
               :total="wineAttributes.length"
               />
    <v-btn @click="submit">診断結果を表示する</v-btn>
  </v-layout>
</template>

<script lang="ts">
  import Vue from 'vue'
  // nuxt-class-componentというライブラリで
  // デコレータでVuexの関数を持ってこれる
  import Component, {Action, Getter, Mutation} from 'nuxt-class-component'
  @Component({
  components: {
    FormCard
  }})
  export default class Index extends Vue {
    // この形式で宣言するとGettersを呼び出し、ワインの属性値を
    // が設定できる
    @Getter('GET_WINE_ATTR') wineAttributes: WineAttribute[]
    (省略)
  }
//}

Vuexの関数をコンポーネントに組み込むことで、アプリにデータを注入し
アプリの形が出来上がっていきます。

//footnote[webpack][もちろん、パフォーマンスチューニングや本格的なパッケージングを行う場合は、Nuxt.jsでもwebpackの知識は必要になります。]
//footnote[tslint][TypeScriptのlintは、eslintというJS用のlinterをts用にコンバージトしたものか、ts用のtslintを利用、２つの方法があります。]
//footnote[why_hs][TypeScriptだと型のエラーで、コンパイルが通らなかったため]
