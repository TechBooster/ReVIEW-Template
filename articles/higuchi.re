
= Rspec+GitHubActions+DataStudioを使ったカバレッジ推移の可視化
== 背景と目的
品質向上のためには、テストを書くことが重要です。
しかし、スケジュールに追われる中で、つい手を抜いてしまいがちな部分ではあります。
カバレッジの時系列推移を可視化することで、楽しんでテストを書ける環境を整えました。
本文章では、その手法について解説します。

== SimpleCovからカバレッジ情報取得
Ruby on Railsを使ったシステム開発を行っており、Rspecでテスト実装しています。
また、カバレッジはSimpleCovを使って取得しています。
SimpleCovにはFormatterという機能があり、カバレッジ出力を好みの形に整形できます。
Formatterを使って、カバレッジを1行のjsonに出力します。

Formatter classにformat(result)というmethodを作ります。
このresultにカバレッジ情報が詰まっています。
result.groups.source_filesにファイル毎のカバレッジ情報、
result.source_filesに全体の集計結果が格納されています。
今回は合計カバレッジを確認したいので、result.source_filesを利用します。
SimplecovLoggerを作り、この合計カバレッジをLogとして出力します。

//listnum[Formatter][Formatter][ruby]{
class SimpleCov::Formatter::OneLineJsonFormatter
  def format(result)
    logger = ::SimplecovLogger.new('simplecov', DateTime.yesterday)
    logger.info(result.source_files)
  end
end
//}

SimplecovLoggerの実装は以下のようになります。
RailsのApplicationLoggerを継承し、initializeで書き出すファイルをします。
formatterで整形を行っています。
modelにOneLineJsonFormatterで書き出そうとしたカバレッジ情報が入ってきます。
カバレッジの属性には、表の情報が格納されています。

//table[CoverageResult][CoverrageResult]{
属性名 型  説明
--------------------
group_name  String そのファイルの種類(result.groupsのみ)
filename  String  対象ファイル名(result.groupsのみ)
covered_percent Integer 対象ファイルのカバレッジ率(0-100%)
never_lines Integer 対象ファイルのカバレッジ対象外の行数
lines_of_code Integer 対象ファイルの行数
covered_lines Integer 対象ファイルのテストでカバーされた行数
missed_lines  Integer 対象ファイルのテストでカバーされていない行数
//}

これをBigQueryに格納したい形に整形します。
to_jsonで1行のJSONとして書き出すようにしています。
いつ計測されたcoverageか分かるようにtimestampを追加しています。
最後にinfoメソッドをApplicationLoggerからdelegateして、
logger.info()の形で利用できるようにします。

//listnum[Logger][Logger][ruby]{
class SimplecovLogger < ApplicationLogger
  def initialize(table, date)
    super("#{Rails.root}/coverage/simplecov.log")
    self.formatter = formatter()
    self
  end

  def formatter
    Proc.new { |severity, time, progname, model|
      {
        covered_percent: model[:covered_percent],
        never_lines: model[:never_lines],
        lines_of_code: model[:lines_of_code],
        covered_lines: model[:covered_lines],
        missed_lines: model[:missed_lines],
        time: @date.strftime('%s').to_i,
      }.to_json.to_s + "\n"
    }
  end

  class << self
    delegate :info, to: :instance
  end
end
//}

ここまでで作ったOneLineJsonFormatterをSimpleCovに設定します。
設定はspec/spec_helper.rbで行います。
ここに以下の記載を追記します。

//listnum[SpecHelper][SpecHelper][ruby]{
SimpleCov.start 'rails' do
  require_relative 'support/one_line_json_formatter.rb'
  SimpleCov.formatter = SimpleCov::Formatter::OneLineJsonFormatter
end
//}

その後テストを実行すると、指定したファイルに行ごとのJSONが出力されます。

//cmd{
$ rspec
$ tail -1 ./coverage/simplecov.log
  {"covered_percent":100.0,"never_lines":9,"lines_of_code":5,"covered_lines":5,"missed_lines":0,"time":1660003200}
//}


== GitHubActionsを使ったカバレッジ情報送信
ここまでの変更でテスト実行後にカバレッジ情報がファイル出力されるようになりました。
ここでは、ファイル出力されたカバレッジ情報をBigQueryへ送信します。

まずは、BigQueryに格納するテーブルを作ります。
Schemaをjsonで以下のように記載します。

//listnum[simplecov][simplecov][JSON]{
[
  { "name": "covered_percent", "type": "INTEGER" },
  { "name": "never_lines", "type": "INTEGER" },
  { "name": "lines_of_code", "type": "INTEGER" },
  { "name": "covered_lines", "type": "INTEGER" },
  { "name": "missed_lines", "type": "INTEGER" },
  { "name": "time", "type": "INTEGER" }
]
//}

bqコマンドを使って、テーブルを作成します。
日毎にパーティションを切り、DB:aumo_mediaにsimplecovと言うテーブルを作ります。

//cmd{
$ bq mk -t --time_partitioning_type DAY aumo_media.simplecov simplecov.json
//}

作ったテーブルにデータを格納します。
GitHubでPull Requestを作ると、GitHub Actionsでテストが回るように設定しています。
そこで、rspec実行後にBigQueryへ転送する処理を追記します。


secretsの設定方法を説明
設定画面のスクショ
GitHub Actionsの設定を説明

//listnum[SpecHelper][SpecHelper][yml]{
  - name: Authenticate GCP
    uses: google-github-actions/auth@v0
    with:
      credentials_json: '${{ secrets.GCP_SERVICE_ACCOUNT }}'

  - name: setup GCP
    uses: google-github-actions/setup-gcloud@v0

  - name: send coverage to BigQuery
    run: |
      bq load --source_format NEWLINE_DELIMITED_JSON aumo_media.simplecov tea-app/coverage/simplecov.log
//}


確認方法の説明
BigQuery画面のスクショ

== DataStudioを使った時系列グラフ作成
DataStudioはGoogleが提供する無料のBIツールです。
これを使って、BigQueryに格納されたカバレッジデータを時系列グラフとして表示します。

まずは、ブラウザからDataStudio (https://datastudio.google.com/) にアクセスします。
Google Accountへのログインが求められるので、BigQueryのユーザ権限のあるユーザでログインします。
以下のようなホーム画面が表示されるので、「空のレポート」をクリックします。

DataStudioホーム画面のスクショ

すると空のシートができます。まずは、BigQueryへの接続設定を行います。

BigQueryコネクタのスクショ

次にグラフを追加します。

グラフ作成画面のスクショ


== まとめ
カバレッジの時系列グラフを作成することができました。
GitHub ActionsでRspecを回している方なら、2時間程度でグラフを作成できると思います。
Formatterに渡されるデータには、ファイル毎のカバレッジ情報も含まれています。
それを使うことで、より詳細な分析も可能です。
この記事がお役に立てれば幸いです。
