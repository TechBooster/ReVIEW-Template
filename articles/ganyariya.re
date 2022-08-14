= まるでわからないため 必要最低限な DI コンテナを作ってみる

こんにちは！ 私は 2022 年から WFS 社の新卒サーバーサイドエンジニアとして働かせていただいている @<href>{https://twitter.com/ganyariya,@ganyariya} です。
チームに配属されて以降は、バックエンドの API 開発・運用などを行いながら、先輩方に追いつこうと日々勉強中です。

さて、私が所属するサーバサイドチームでは PHP のフレームワークである @<href>{https://www.slimframework.com/,Slim} を用いてバックエンド API を開発しています
（より詳細なサーバサイドの技術については弊社の技術ブログ@<fn>{ganyariya-wfs-note}をご参考ください）。

この Slim は非常にシンプルなマイクロフレームワークです。
主に HTTP Request/Response とルーティング設定にのみ Slim が用いられています。
モデルの生成や Database クライアントなどについては、他のライブラリや自社で開発したツールなどが適宜用いられています。

チームに配属されてからはじめて Slim で API を開発したとき、私ははじめて次のようなコードと出会いました。

//list[ganyariya-slim-di][Slim の設定ファイルにおける DI (Dependency Injection)][php]{
<?php

namespace ...;

use DI;
use ...;

return [
    MasterRepositoryInterface::class => DI\get(MasterRepository::class),
    UserBookRepositoryInterface::class => DI\get(UserBookRepository::class),
];
//}

開発経験が豊富なエンジニアの方々にとってはよく見るものである気がします。
しかし、私にとっては、なんなんだこれは...となりました。
というのも 私は主に競技プログラミングやゲーム開発 (Siv3D) などを学生のころは行っており、本格的な Web 開発は初めてだったためです。

その後この不思議な呪文について調べてみると、@<kw>{DI コンテナ}を用いた @<kw>{依存性注入 (DI: Dependency Injection)} なるものをこのコードは行っていました。
具体的には PHP-DI というライブラリをもちいて DI コンテナを作成し、このコンテナを Slim に渡すことで、@<kw>{依存性}を@<kw>{注入}しながら自動でルーティングをしてくれるそうです @<fn>{ganyariya-slim-di-container}。

分からないながら開発をしていくなかで DI コンテナというものはいい感じにオブジェクトを生成して他のオブジェクトの引数に渡してくれるんだなぁとなんとなく分かってはきました。
しかし、このままでは曖昧な理解で終わってしまいます。

そこで今回は @<kw>{依存} / @<kw>{依存性注入 (DI)} / @<kw>{DI コンテナ} について、 必要最低限の機能を伴った DI コンテナを作成することで理解を深めてみようと思います。

== 対象読者・内容

=== 対象読者

対象読者としては以下を想定しています。

 * 依存・依存性注入をはじめて耳にした方
 * DI コンテナを自作してみたい方

=== 内容

==== 扱うこと

 * 依存ならびに依存性注入の概要
 * DI コンテナの説明
 * 自作 DI コンテナの説明

==== 扱わないこと

 * DI を利用するメリット
 * Clean Architecture
 * DI コンテナ vs Service Allocator Pattern

最後に、私の技術に関する知識不足から誤った記載・理解が含まれることが想定されます。
こちらご了承いただければと思います。

== 依存

Dependency Injection Container (DI コンテナ) を理解・自作するためにはまずは @<kw>{依存} を理解する必要があります。
この依存とは一体なんでしょうか。

ある要素 A を 実装するときに 他の要素 B が必要となる場合 A は B に依存しているといえます。
ここで、要素とはクラスやライブラリ、モジュール、ミドルウェアなどシステム開発に必要な多くのものが当てはまります。
できるだけ要素の依存関係を弱くし、要素間の結合度を下げることで、開発しやすくテストしやすい状態を保てます。

文章では少し分かりづらいため、サンプルコードで説明します。
この例では、クラス A はクラス B がない限り生成できません。この場合、 A は B に依存しているといえます。

//list[ganyariya-dependency][依存][php]{
<?php

class B
{
}

class A
{
    private B $b;
    public function __construct()
    {
        $this->b = new B();
    }
}

$a = new A();
var_dump($a);
//}

以降 A が B に依存しているとき、下図のような矢印で表すこととします。

//indepimage[dependency][]

== 依存性注入

続いて、 @<kw>{依存性注入 (DI: Dependency Injection)} について説明します。
といっても 依存性注入は多くの記事で取り扱われているため @<fn>{ganyariya-di-article1} @<fn>{ganyariya-di-article2}、ここでは簡単な説明に留めます。

依存性注入とは、依存性（Dependency: 使用される側のオブジェクトのこと）を外部から注入 (Injection) することです。
使用されるオブジェクト（Dependency）を外部から与えることによって、より疎結合にでき、機能の追加やテストなどが行いやすくなります。

これらについても具体例をもちいて説明していきます。


== DI コンテナ

== 自作 DI コンテナ

=== PSR

=== 自作

== 最後に

次回の技術書展では、より発展させた DI コンテナ、そしてオレオレ Web フレームワークの話もしていきます。

//footnote[ganyariya-wfs-note][https://note.com/wfs_blog/n/n11d137738919]
//footnote[ganyariya-slim-di-container][https://www.slimframework.com/docs/v4/concepts/di.html]
//footnote[ganyariya-di-article1][https://qiita.com/uhooi/items/03ec6b7f0adc68610426]
//footnote[ganyariya-di-article2][http://blog.a-way-out.net/blog/2015/08/31/your-dependency-injection-is-wrong-as-I-expected/]

#@# https://qiita.com/uhooi/items/03ec6b7f0adc68610426
#@# 
#@# 
#@# 
#@# 
#@# 
#@# 
#@# 
#@# 
#@# 
#@# 
#@# 
#@# 
#@# 
#@# 
#@# 