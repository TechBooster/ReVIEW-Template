= AIアプリをデプロイしよう
では、今まで作ったアプリを本番にデプロイしてみましょう。

== 今回の構成
今回は、次のような構成で動作環境を構築していきます。
開発者がプッシュしただけで、本番環境までデプロイされるCD環境を作ることを目標にします。
//image[figure1][今回の動作環境]

それぞれの要素について説明していきます。

=== GitHub
世界最大のソースコード管理サービスであり
かつもっとも使いやすいサービスです。同様のサービスとしてBitbucketやGitlabがありますが今回はGitHubを用います。

=== CircleCI
継続的デプロイ/デリバリを行うサービスとして、GitHubと組み合わせて使われることが多いサービス。
GitHubとの連携が非常に簡単ですので、今回はこちらを利用します。

=== Google Kubernetes Engine（GKE）
実際に動かす環境として、Google Cloud Platformのサービスのひとつである「Google Kubernetes Engine」を用います。

== 開発環境のDocker化
== CircleCIの設定
== GKEの設定
== GKEのCluster作成
//cmd{
gcloud container clusters create nullsuck --num-nodes 2 --zone asia-northeast1-a
//}