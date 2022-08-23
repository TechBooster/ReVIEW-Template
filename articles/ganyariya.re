= 必要最低限な DI コンテナを作ってみる

こんにちは！ 私は 2022 年から WFS 社の新卒サーバーサイドエンジニアとして働かせていただいている @<href>{https://twitter.com/ganyariya,@ganyariya} です。
チームに配属されて以降は、バックエンドの API 開発・運用などを行いながら、先輩方に追いつこうと日々勉強中です。

さて、私が所属するサーバサイドチームでは PHP のフレームワークである @<href>{https://www.slimframework.com/,Slim} を用いてバックエンド API を開発しています
（より詳細なサーバサイドの技術については弊社の技術ブログ@<fn>{ganyariya-wfs-note}をご参考ください）。

この Slim は非常にシンプルなマイクロフレームワークです。
主に HTTP Request/Response とルーティング設定にのみ Slim が用いられています。
モデルの生成や Database クライアントなどについては、他のライブラリや自社で開発したツールなどが適宜用いられています。

チームに配属されてから Slim で API を開発したとき、私は次のようなコードとはじめて出会いました。

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

開発経験が豊富なエンジニアの方々にとってはよく見るものでしょう。
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

これらについても、「様々な種類の猫を集めて育てるゲーム」を題材にサンプルコードで説明します（猫を育てるゲームがやりたいので）。

==== 強い依存

猫のガチャを引く処理である GachaDrawInteractor は、 ユーザが持っている猫を扱う UserCatRepository に強く依存しています。
ここで、 Repository や Interactor の用語については Clean Architecture を調べていただければと思います。

このままでは、 GachaDrawInteractor を生成すると必ず新しい UserCatRepository が生成されます。
UserCatRepository に設定情報などを与えられないため、テストでも本番と同じ MySQL インスタンス (xxx.xxx.xxx.xxx) にアクセスすることとなります。

//list[ganyariya-strong-dependency][強い依存][php]{
class UserCatRepository
{
    private array $config;
    // MySQL で実装されている
    public function __construct()
    {
        $this->config = ['host' => 'xxx.xxx.xxx.xxx'];
    }
}
class GachaDrawInteractor
{
    private UserCatRepository $userCatRepository;
    public function __construct()
    {
        $this->userCatRepository = new UserCatRepository();
    }
    public function handle()
    {
        // ガチャを引く処理
    }
}
//}

==== 依存性注入を用いた場合

依存性注入を用いると以下のように表せます。
UserCatRepository は外部から設定ファイルを受け取ることができ、テスト用の MySQL インスタンスなどを指定できます。
GachaDrawInteractor も UserCatRepository を継承したクラスなどを引数として受け取ります。

外部からの注入を許可することによって、プログラムをより柔軟に変更できるようになります。

//list[ganyariya-dependency-injection][依存性注入][php]{
class UserCatRepository
{
    private array $config;
    // MySQL で実装されている
    public function __construct(array $config)
    {
        $this->config = $config;
    }
}
class GachaDrawInteractor
{
    private UserCatRepository $userCatRepository;
    public function __construct(UserCatRepository $userCatRepository)
    {
        $this->userCatRepository = $userCatRepository;
    }
}
//}

==== IoC (制御の反転)

しかし、上記の実装では Application Business Rule (ABR) に含まれる GachaDrawInteractor が、
より変更がされやすく重要度が低い Interface Adapters の UserCatRepository に依存してしまっています。

プロダクトとして重要なのは Domain Object ならびに UseCase です。
これらが、ミドルウェアやフレームワークに依存してしまっている状態はよくありません。
たとえば、 MySQL から Spanner にデータベースを移行した場合、 GachaDrawInteractor が 直接 UserCatRepository に依存しているせいで、 GachaDrawInteractor 自体もコードの修正が必要になる可能性があります。
また、分かりやすい問題としては、テストも必ず MySQL で実行する必要があり時間が多くかかる可能性があります。できればインメモリを利用した StubRepository を実装することにで高速にテストを終了させ MySQL 自体への依存も避けたいです。

そこで、@<kw>{制御の反転 (IoC: Inversion of Control)} を行います @<fn>{ganyariya-ioc}。

UserCatRepositoryInterface をプロダクト側で用意し、 Interactor は UserCatRepositoryInterface を受け取るようにします。
これによって、猫ガチャを引く Interactor は、中身の実装は不明だが、ある猫の取得・すべての猫の取得・ある猫の削除を行えるようになります。

テストを実行するときは UserCatRepositoryInterface を実装したインメモリ StubRepository を利用すれば高速にテストできます。
また、 MySQL から Spanner に移行するときも、UserCatRepositoryInterface を実装した UserCatSpannerRepository を用意すればよいです。
このとき、 GachaDrawInteractor 側の変更は必要ありません。

//list[ganyariya-ioc][制御の反転][php]{
interface UserCatRepositoryInterface
{
    public function find($id): UserCat;
    public function findAll(): array;
    public function delete($id): void;
}
class UserCatRepository extends UserCatRepositoryInterface
{
    // find, findAll, delete を実装する
}
class GachaDrawInteractor
{
    private UserCatRepositoryInterface $userCatRepository;
    public function __construct(UserCatRepositoryInterface $userCatRepository)
    {
        $this->userCatRepository = $userCatRepository;
    }
}
//}

==== 依存ツリー

依存性注入と IoC によって、より柔軟に外部からオブジェクトを注入できるようになりました。
しかし、実際にオブジェクトを注入するためにはどこかで生成、すなわちコンストラクタを呼び出す必要があります。
このとき、複数の要素が相互に依存しあっている場合、適切に注入することが困難となります。

以下の図で示すような依存関係があったとします（複雑ですね）。

//indepimage[dependency_tree][]

このとき、 C を生成する場合を考えてみます。
愚直に実装すると以下のようなコードになります。
C を生成するために K を何度も生成しており、無駄にメモリを消費してしまいます。

また、 @<code>{$k = new K();} とすれば K を再利用して C や G を生成できますが、自らの手でボトムアップに依存関係を考慮してコードを書く必要があります。
ある 1 箇所にコンストラクタの生成が集まっているならまだ良いですが、他の場所でもコンストラクタを呼び出す場合収集がつかなくなってきます。

//list[ganyariya-tree][依存ツリー][php]{
$c = new C(new E(), new F(new H(new K()), new I(new K()), new J(new K())));
//}

この問題を解決するために、依存関係の構造に着目します。
依存関係は有向グラフかつ閉路がない、つまり非巡回有向グラフ (DAG) であるといえます。
DAG の場合、根のノードから DFS の要領で順番に要素にアクセスすることで、依存性を自動的に解決できそうです。
また、解決したオブジェクト (@<code>{$c, $k} など)をキャッシュしておけば、ノードの数を N, 辺の数を M とすれば、計算量が O(N + M) に抑えられます。 

この DFS を利用し、かつ解決したオブジェクトを保存し適宜取り出すものが DI コンテナです（ここまで来るまで長かった）。
それでは、実際に DI コンテナの例を見ていき、自作していきます。

== DI コンテナ

DI コンテナ は、依存性注入を手助けするツールです。
手助けするとあるように、依存性注入を行うために DI コンテナが必ず必要となるというわけではありません。
DI コンテナがあるとより手軽に依存性注入を実現できるというだけにすぎません。

具体的には、DI コンテナは @<code>{{"key": "value"}}の形式で、オブジェクトを取り出すためのキーと、そのオブジェクト自体を紐付けて保存しています。

以下は DI コンテナのイメージ例です。 "hello" というキーに対して、 "world" というオブジェクト（値）を格納しています。 
また、 UserBookRepositoryInterface::class というキーに対して、 Interface を実装した UserCatRepository のオブジェクトを格納しています。

//list[ganyariya-di-container][DIコンテナのイメージ][php]{
$container = new Container();
$container->set("hello", "world!");
assert($container->get("hello"), "world");

$container->set(UserCatRepositoryInterface::class, new UserCatRepository());
assert($container->get(UserCatRepositoryInterface::class) instanceof UserCatRepository);
//}

また、DI コンテナライブラリによっては、自動的に依存性を解決し (autowire)、自分で依存性を解決しなくてよいものもあります。
詳しくはこちらのテストコードを参考いただきたい@<fn>{ganyariya-di-autowire-testcode}ですが、以下のように Interface に対してその Interface を実装したクラスを指定するだけで自動で依存性を解決してくれます。

//list[ganyariya-di-container-autowire][DIコンテナ autowire][php]{
$container->set(GetsInterface::class, Hako\fetch(GetsInteractor::class));
$container->set(MasterRepositoryInterface::class, Hako\fetch(MasterRepository::class));
$container->set(UserRepositoryInterface::class, Hako\fetch(UserRepository::class));
//}

上記コードのあとで @<code>{$container->get(GetsInterface::class);} のように書くと、GetsInteractor のコンストラクタ情報を解析して必要なオブジェクトの依存性を再帰的に解決してインスタンスを生成してくれます。
また、自動的に生成された GetsInteractor インスタンスをキャッシュするため、他オブジェクトが GetsInteractor インスタンスを利用しようとすると、キャッシュされていた GetsInterface を取得します。
これによって、無駄なオブジェクトを生成するということを避けられます（必ず新規作成したい場合は、そのような設定を別途行えます）。

DI コンテナをより詳しく知るには、 PHP における DI コンテナライブラリとして有名な PHP-DI @<fn>{ganyariya-php-di} を参考にしてください。

== 自作 DI コンテナ

それでは実際に自作 DI コンテナを作っていきます。
DI コンテナを自作することで、 DI コンテナが内部で何を主に行っているかを理解していきます。

PHP における DI コンテナライブラリの多くは、 PSR という規約に従っています。
今回自作する DI コンテナも PSR に従うことにします。

=== PSR

PSR (PHP Standards Recommendations) @<fn>{ganyariya-php-psr} は PHP-FIG (The PHP Framework Interoperability Group) @<fn>{ganyariya-php-fig} が定めている PHP フレームワークにおける標準規約です。

PHP-FIG は PHP フレームワークやライブラリのメンテナの方々らが立ち上げた団体であり、 PHP のエコシステムを発展させて、より優れた標準規約を推進することを目的にしています。
PHP フレームワーク・ライブラリの開発者による相互運用組織であり、 PHP 公式の団体ではありません。

また、 PSR は PHP-FIG が策定した規約です。
具体的には、フレームワークを相互運用するために必要なインターフェースなどが、内容ごとに番号を分けて規定されています。
例えば、 PSR-1 は標準的なコーディングルールであり、 PSR-7 は HTTP でやり取りするメッセージの Interface を規定しています。

これらの規約はあくまで PHP-FIG の開発者の間で取り決めたものであり、その他の PHP を利用しているエンジニアは PSR に従う必要はありません。
しかし、開発をする上ではじめからルールがあったほうが嬉しいため、多くのエンジニア・組織は好きな PSR のルールとインターフェースを取り入れてコーディングしています。

DI コンテナについても PSR-11 @<fn>{ganyariya-php-psr-11} でインターフェースが取り決められています。
@<kw>{Psr\\Container\\ContainerInterface} が定義されており、 @<code>{get, has} というメソッドのみ持ちます。
get では識別子に一致するオブジェクトを返し、ないなら @<code>{NotFoundExceptionInterface} を実装した例外を返します。
また、 has は識別子に一致するオブジェクトがあるなら true を返し、ないなら false を返します。

PSR-11 のインターフェースはこれだけしか、つまり @<kw>{識別子・キーに対するオブジェクトの取り出し方}しか定義されていません。
よって、 map で key と value を内部で管理するという辞書構造そのままの DI コンテナを作ったとしても get, has を定義しておけば PSR-11 を満たしたといえます（簡単ですね）。

ただし、取り出し方は定められている一方、@<kw>{識別子・キーに対する値の設定方法}や@<kw>{依存の解決方法}は定義されていません。
よって、どのように識別子にオブジェクトを設定するか、依存性を解決するかは DI コンテナライブラリごとの実装に依存しています。

今回作成する自作 DI コンテナは、 PHP-DI @<fn>{ganyariya-php-di} のような識別子に対する値の設定方法を実装します。
ただし、 PHP-DI の内部実装を読んだわけでなく、 @<code>{DI\\get} のようなメソッド名や機能のみ参考にしているため、内部的な構造は大きく異なる可能性が高いことに注意してください。

=== 自作

== 最後に

次回の技術書展では、より発展させた DI コンテナ、そしてオレオレ Web フレームワークの話もしていきます。

//footnote[ganyariya-wfs-note][https://note.com/wfs_blog/n/n11d137738919]
//footnote[ganyariya-slim-di-container][https://www.slimframework.com/docs/v4/concepts/di.html]
//footnote[ganyariya-di-article1][https://qiita.com/uhooi/items/03ec6b7f0adc68610426]
//footnote[ganyariya-di-article2][http://blog.a-way-out.net/blog/2015/08/31/your-dependency-injection-is-wrong-as-I-expected/]
//footnote[ganyariya-ioc][https://ja.wikipedia.org/wiki/%E5%88%B6%E5%BE%A1%E3%81%AE%E5%8F%8D%E8%BB%A2]
//footnote[ganyariya-di-autowire-testcode][https://github.com/ganyariya/Hako/blob/99d29f76bad594e3fca81eaa9cbe8e0488eaffb8/tests/HogameTest.php]
//footnote[ganyariya-php-di][https://php-di.org/doc/understanding-di.html]
//footnote[ganyariya-php-fig][https://www.php-fig.org/bylaws/]
//footnote[ganyariya-php-psr][https://www.php-fig.org/psr/]
//footnote[ganyariya-php-psr-11][https://www.php-fig.org/psr/psr-11/]


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