= 必要最低限な DI コンテナを作ってみる

こんにちは！ 私は 2022 年から WFS 社の新卒サーバーサイドエンジニアとして働かせていただいている @ganyariya @<fn>{ganyariya-twitter} です。
チームに配属されて以降は、バックエンドの API 開発・運用などを行いながら、先輩方に追いつこうと日々勉強中です。

さて、私が所属するサーバサイドチームでは PHP のフレームワークである Slim @<fn>{ganyariya-slim-di-container} を用いてバックエンド API を開発しています
（より詳細なサーバサイドの技術については弊社の技術ブログ@<fn>{ganyariya-wfs-note}をご参考ください）。

この Slim は非常にシンプルなマイクロフレームワークです。
主に HTTP Request/Response とルーティング設定にのみ Slim が用いられています。
モデルの生成や Database クライアントなどについては、他のライブラリや自社で開発したツールなどが適宜用いられています。

チームに配属されてから Slim で API を開発したとき、私は@<list>{ganyariya-slim-di}のようなコードとはじめて出会いました。

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

文章では少し分かりづらいため、サンプルコード @<list>{ganyariya-dependency}で説明します。
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

以降 A が B に依存しているとき、@<img>{dependency} のような矢印で表すこととします。

//image[dependency][依存性を表す矢印]{
//}

== 依存性注入

続いて、 @<kw>{依存性注入 (DI: Dependency Injection)} について説明します。
といっても 依存性注入は多くの記事で取り扱われているため @<fn>{ganyariya-di-article1} @<fn>{ganyariya-di-article2}、ここでは簡単な説明に留めます。

依存性注入とは、依存性（Dependency: 使用される側のオブジェクトのこと）を外部から注入 (Injection) することです。
使用されるオブジェクト（Dependency）を外部から与えることによって、より疎結合にでき、機能の追加やテストなどが行いやすくなります。

これらについても、「様々な種類の猫を集めて育てるゲーム」を題材にサンプルコードで説明します（猫を育てるゲームがやりたいので）。

==== 強い依存

@<list>{ganyariya-strong-dependency} の例では、猫のガチャを引く処理である GachaDrawInteractor は、 ユーザが持っている猫を扱う UserCatRepository に強く依存しています。
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

依存性注入を用いると@<list>{ganyariya-dependency-injection}のように表せます。
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

しかし、@<list>{ganyariya-dependency-injection}の実装では Application Business Rule (ABR) に含まれる GachaDrawInteractor が、
より変更がされやすく重要度が低い Interface Adapters の UserCatRepository に依存してしまっています。

プロダクトとして重要なのは Domain Object ならびに UseCase です。
これらが、ミドルウェアやフレームワークに依存してしまっている状態はよくありません。
たとえば、 MySQL から Spanner にデータベースを移行した場合、 GachaDrawInteractor が 直接 UserCatRepository に依存しているせいで、 GachaDrawInteractor 自体もコードの修正が必要になる可能性があります。
また、分かりやすい問題としては、テストも必ず MySQL で実行する必要があり時間が多くかかる可能性があります。できればインメモリを利用した StubRepository を実装することにで高速にテストを終了させ MySQL 自体への依存も避けたいです。

そこで、@<kw>{制御の反転 (IoC: Inversion of Control)} を行います @<fn>{ganyariya-ioc}。

@<list>{ganyariya-ioc} では、UserCatRepositoryInterface をプロダクト側で用意し、 Interactor は UserCatRepositoryInterface を受け取るようにします。
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

@<img>{dependency_tree}で示すような依存関係があったとします（複雑ですね）。

//image[dependency_tree][複雑な依存関係（依存ツリー）]{
//}

このとき、 C を生成する場合を考えてみます。
愚直に実装すると@<list>{ganyariya-tree}のようなコードになります。
C を生成するために K を何度も生成しており、無駄にメモリを消費してしまいます。

//list[ganyariya-tree][依存ツリー][php]{
$c = new C(new E(), new F(new H(new K()), new I(new K()), new J(new K())));
//}

また、 @<code>{$k = new K();} とすれば K を再利用して C や G を生成できますが、自らの手でボトムアップに依存関係を考慮してコードを書く必要があります。
ある 1 箇所にコンストラクタの生成が集まっているならまだ良いですが、他の場所でもコンストラクタを呼び出す場合収集がつかなくなってきます。

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

具体的には、DI コンテナは @<code>{{"key":"value"}}の形式で、オブジェクトを取り出すためのキーと、そのオブジェクト自体を紐付けて保存しています。

@<list>{ganyariya-di-container}は DI コンテナのイメージ例です。 "hello" というキーに対して、 "world" というオブジェクト（値）を格納しています。 
また、 UserBookRepositoryInterface::class というキーに対して、 Interface を実装した UserCatRepository のオブジェクトを格納しています。

//list[ganyariya-di-container][DIコンテナのイメージ][php]{
$container = new Container();
$container->set("hello", "world!");
assert($container->get("hello"), "world");

$container->set(UserCatRepositoryInterface::class, new UserCatRepository());
assert($container->get(UserCatRepositoryInterface::class) instanceof UserCatRepository);
//}

また、DI コンテナライブラリによっては、自動的に依存性を解決し (autowire)、自分で依存性を解決しなくてよいものもあります。
詳しくは Hako のテストコードを参考いただきたい@<fn>{ganyariya-di-autowire-testcode}ですが、@<list>{ganyariya-di-container-autowire}のように Interface に対してその Interface を実装したクラスを指定するだけで自動で依存性を解決してくれます。

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

今回作成する自作 DI コンテナは、 PHP-DI @<fn>{ganyariya-php-di} のような設定方法を実装します。
ただし、 PHP-DI の内部実装を読んだわけでなく、 @<code>{DI\\get} のようなメソッド名や機能のみ参考にしているため、内部的な構造は大きく異なる可能性が高いことに注意してください。

=== 自作

今回自作した DI コンテナライブラリは @<kw>{Hako} @<fn>{ganyariya-hako} です。
使い方や詳しい実装は README ならびにソースコードとテストコードを参考にしてください。

==== ひとまず array にオブジェクトを入れる

PSR-11 では @<kw>{Psr\\Container\\ContainerInterface} を定義しているため、このインターフェースを実装した最も簡単なコンテナクラス Container.php @<fn>{ganyariya-hako-container} を作ります。
ソースコードの一部のみ@<list>{ganyariya-hako-container}に掲載しています。
ある key(id) に対してオブジェクト value を紐付けているだけです。

//list[ganyariya-hako-container][最もシンプルな Conainer][php]{
// 省略
use Psr\Container\ContainerInterface;
class Container implements ContainerInterface
{
    private array $data;
    public function __construct()
    {
        $this->data = [];
    }
    public function get(string $id): mixed
    {
        if (!$this->has($id)) throw new ContainerException("Not Found: $id");
        return $this->data[$id];
    }
    public function has(string $id): bool
    {
        return array_key_exists($id, $this->data);
    }
}
//}

==== set と fetch 

さきほどの Container では、get, has で値を取り出せますが、値を設定できません。
そのため、@<list>{ganyariya-hako-set}のように、set(id, value) で値を設定できるようにします@<fn>{ganyariya-hako-set}。

//list[ganyariya-hako-set][set][php]{
    public function set(string $id, mixed $value): void
    {
        $this->data[$id] = $value;
    }
//}

また、 Fetcher / fetch というヘルパークラス・メソッドを用意することによって、指定した id をコンテナから取り出せるようにします @<fn>{ganyariya-hako-fetcher}。
これらは PHP-DI における DI\get と同じ機能を持ちます。 @<list>{ganyariya-di-container-autowire} のような autowire を今後行うために今のうちに用意しています。

@<kw>{注意点として、この時点ではすべての依存性を自分でコードを書いて設定しなくてはなりません}。
たとえば、何もコンストラクタを取らない @<code>{A} というクラスがあったとします。
このとき @<code>{$container->set(A::class, new A());} と必ず自分の手で設定する必要があります。
クラス名から自動で依存性を解決するためには、後述する DFS と Reflection による自動解決を実装しなければなりません。

==== ContainerClosure

次に、ContainerClosure と Hako で呼称している機能を追加しました@<fn>{ganyariya-hako-container-closure}。
@<list>{ganyariya-container-closure} のように、ごにょごにょ操作をしたオブジェクトを返したい場合に利用します。
@<code>{function(ContainerInterface)} が value に入っているかどうかは ReflectionFunction を使って判定しています。

//list[ganyariya-container-closure][ContainerClosure][php]{
$container->set(UserRepositoryInterface::class, function(ContainerInterface $c) {
    /**
     * ここで色々追加で操作をする
     * config['localhost']を取得したり時間操作をするなど自由に操作をする
     */

    return new UserRepository(
        $c->get(MasterRepositoryInterface::class);
    );
});
//}

==== Cache

続いて、 Cache クラスを導入し一度解決した依存性をキャッシュするようにします @<fn>{ganyariya-hako-cache}。
これによって、あるクラスのインスタンスが複数作成されることを防げます。
詳しい実装は @<fn>{ganyariya-hako-cache} をご参考ください。

実装としてはシンプルで、@<code>{set(id, value)} で与えられる value を Cache インスタンスの内部に actualData として格納します。
そして、依存性が解決されたタイミングで、解決されたオブジェクトを resolvedData として保存します。

==== DFS と Reflection による依存性解決

それでは DFS と Reflection による依存性解決を実装してきます。
この機能を実装すると @<list>{ganyariya-di-container-autowire} で示すように、インターフェース id とそのインターフェースを実装したクラス value を指定するだけで自動で依存性解決をしてくれるようになります。
くわえて、インターフェース以外、つまりすべてのクラスの依存性が自動で解決されるようになります。
まとめると、@<kw>{あるインターフェースに対してどのクラスを返すかのみ指定するだけで、それ以外はすべて自動で解決されます}（便利ですね）。

ここからは説明が難しいため、@<list>{ganyariya-di-container-autowire} の例を用いて依存性解決の流れをたどりながら説明します。
@<list>{ganyariya-di-container-autowire}のあとに @<code>{$container->get(GetsInterface::class)} を実行したとします。
すると、@<code>{GetsInterface::class} に紐付いている @<code>{GetsInteractor::class} をコンテナ内で探します。

しかし、@<code>{GetsInteractor::class} は明示的に set されていません @<fn>{ganyariya-hako-dfs-1}。
そのため、@<code>{this->get($id)} が false となり、@<code>{$this->dynamicLoader->existsDeclaredClass($id)} で DynamicLoader のメソッドが呼ばれます。

ここで、DynamicLoader は依存性を動的に解決するヘルパークラスです。
@<code>{existsDeclaredClass} で指定された id が autoload 名前空間に存在するかを調べます @<fn>{ganyariya-hako-dfs-2}。
存在するのであればクラス情報を Reflection で取得し、動的に依存性を解決・オブジェクトを生成します。
@<code>{GetsInteractor::class} は名前空間に存在するため true になり、依存性の動的解決フェーズに移ります。

依存性の動的解決として DynamicLoader の load が呼ばれます@<fn>{ganyariya-hako-dfs-3}。
コードを見ていただくとわかりますが、 ReflectionClass を用いてコンストラクタのパラメータ情報を取り出しています（ReflectionHelperというヘルパークラスに名前を取得する処理を切り分けています）。
そして、コンストラクタのパラメータ名で再帰的に get し依存性を解決します@<fn>{ganyariya-hako-dfs-4}。
ここが DFS の部分となります。

再帰的に取得したインスタンスをもとに @<code>{$generated = new $id(...$arguments)}で @<code>{GetsInteractor::class} のインスタンスを生成しキャッシュします。
これで、明示的に依存性を指定しなくても名前から自動で解決してくれるようになりました@<fn>{ganyariya-hako-dfs-5}。

これらの依存性の解決方法は再帰的に行われるため、流れが追いづらいものとなっています。
テストコードをお読みいただき、fetch, cache, loader がどう呼ばれているかを追っていただくと良さそうです。

==== addDefinitions

最後に、よりシンプルに DI コンテナを作成できるように ContainerBuilder を用意しました@<fn>{ganyariya-hako-container-builder}。
set を使用するかわりに、配列を指定することで設定を行えます。

== 最後に

後半はかなり駆け足になってしまいましたが、依存性注入と DI コンテナ、そして自作 DI コンテナについて書かせていただきました。
PHP における DI コンテナがどのように依存性を解決しているのかを、自分の手でライブラリを書くことでより理解できました。
懸念点として、Slim などの Web フレームワークとまだ組み合わせていないため、 Hako で設定した DI コンテナが Slim などで動くかどうかを試そうと思います。

次回の技術書展では、 Go 言語で以下のどれかを自作してまとめようと思います。

 * Micro Web Framework
 * Git System
 * Database
 * 自作コンパイラ

ここまでお読みいただきありがとうございました。

//footnote[ganyariya-twitter][https://twitter.com/ganyariya]
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
//footnote[ganyariya-hako][https://github.com/ganyariya/Hako]
//footnote[ganyariya-hako-container][https://github.com/ganyariya/Hako/blob/68889089286a3addf78f90b0b8bfd6ed0a1272c6/src/Container.php]
//footnote[ganyariya-hako-set][https://github.com/ganyariya/Hako/blob/cf02f4d851ec1260302f9c207373941f80dbbece/src/Container/Container.php#L23]
//footnote[ganyariya-hako-fetcher][https://github.com/ganyariya/Hako/blob/cf02f4d851ec1260302f9c207373941f80dbbece/src/Fetcher/Fetcher.php]
//footnote[ganyariya-hako-container-closure][https://github.com/ganyariya/Hako/commit/fca4a6a37723cf43694608abb8d4ebcdbf816abb]
//footnote[ganyariya-hako-cache][https://github.com/ganyariya/Hako/commit/fca4a6a37723cf43694608abb8d4ebcdbf816abb]
//footnote[ganyariya-hako-dfs-1][https://github.com/ganyariya/Hako/blob/v0.1.1/src/Container/Container.php#L39]
//footnote[ganyariya-hako-dfs-2][https://github.com/ganyariya/Hako/blob/v0.1.1/src/Dynamic/Loader.php#L13]
//footnote[ganyariya-hako-dfs-3][https://github.com/ganyariya/Hako/blob/v0.1.1/src/Dynamic/Loader.php#L19]
//footnote[ganyariya-hako-dfs-4][https://github.com/ganyariya/Hako/blob/v0.1.1/src/Dynamic/Loader.php#L30]
//footnote[ganyariya-hako-dfs-5][https://github.com/ganyariya/Hako/blob/v0.1.1/src/Dynamic/Loader.php#L32]
//footnote[ganyariya-hako-container-builder][https://github.com/ganyariya/Hako/blob/v0.1.1/src/Container/ContainerBuilder.php]

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