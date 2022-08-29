
= GitHub Actions でシェルとエディタのベンチマークを取ろう

== はじめに
みなさんは、お気に入りのエディタやシェルはありますか？
普段使っているシェルやエディタを自分好みにカスタマイズしている人も多いのではないでしょうか。
私も zsh や Neovim を自分好みにカスタマイズしており、その設定ファイルは日々複雑化しています。

ところで、みなさんは、シェルやエディタの起動がいつの間にか遅くなってしまった経験はないでしょうか？
シェルやエディタを便利にするために、env 系のツールやプラグインなどを導入し、起動時間に悪影響を与えてしまうことも少なくありません。
複雑化した設定では、どの設定が起動を遅くしているのか見つけるのも一苦労です。

そこで、おすすめなのが、シェルやエディタの起動時間を定期的に計測することです。
これにより、起動が遅くなってしまった原因を見つけやすくすることができます。
本章では、dotfiles @<fn>{guts-dotfiles} を GitHub で管理していることを前提とし、定期的な計測の一手法として GitHub Actions でコミット毎に zsh と Neovim のベンチマークを取る方法を紹介します。

== hyperfine でコマンドのベンチマークを取る

コマンドの起動時間を計測するために、hyperfine というツールを紹介します。

=== hyperfine とは？
hyperfine @<fn>{guts-hyperfine} は Rust で書かれたコマンドのベンチマークツールです。
任意のシェルコマンドの実行時間を計測することができ、複数のコマンドを比較する機能や予めコマンドを実行しておくウォームアップ機能などがあります。

=== hyperfine の使い方

hyperfine の基本的な使い方は下記の通りです。

//list[guts-hyperfine-command-basic][hyperfine のコマンド例][bash]{
hyperfine 'sleep 0.3'
//}

オプションで、ウォームアップ回数や実行回数などを指定することもできます。

//list[guts-hyperfine-result-warm-up-and-runs][hyperfine の実行例 ( ウォームアップ回数: 3, 実行回数: 5 )][bash]{
$ hyperfine 'sleep 0.3' --warmup 3 --runs 5
Benchmark 1: sleep 0.3
  Time (mean ± σ):     310.8 ms ±   0.7 ms    [User: 1.4 ms, System: 2.1 ms]
  Range (min … max):   310.0 ms … 312.0 ms    5 runs
//}

また、実行結果を JSON ファイルに出力することもできます。

//list[guts-hyperfine-result-export-json][hyperfine の実行例 ( JSON ファイルに結果を出力 )][bash]{
$ hyperfine 'sleep 0.3' --warmup 3 --runs 5 --export-json bench.json
Benchmark 1: sleep 0.3
  Time (mean ± σ):     309.8 ms ±   2.3 ms    [User: 1.5 ms, System: 2.2 ms]
  Range (min … max):   305.9 ms … 311.6 ms    5 runs

//}

//list[guts-hyperfine-command-export-json][@<list>{guts-hyperfine-result-export-json} の実行で出力される JSON ファイルの例][json]{
{
  "results": [
    {
      "command": "sleep 0.3",
      "mean": 0.31244138584000003,
      "stddev": 0.0010864612115462355,
      "median": 0.31255536084,
      "user": 0.00182732,
      "system": 0.0029679399999999996,
      "min": 0.31088523584000005,
      "max": 0.31390340284000007,
      "times": [
        0.31255536084,
        0.31215823584,
        0.31390340284000007,
        0.31270469384000005,
        0.31088523584000005
      ],
      "exit_codes": [
        0,
        0,
        0,
        0,
        0
      ]
    }
  ]
}
//}

=== zsh の起動時間の計測
zsh の起動時間を計測するために、下記のコマンドを使用します。
//list[guts-zsh-command][起動時間を計測するための zsh のコマンド][bash]{
zsh -i -c exit
//}
-i でインタラクティブモードで起動し、-c exit で起動後すぐに exit コマンドを実行するようにします。

//list[guts-zsh-result][zsh の起動時間を計測する例][bash]{
$ hyperfine 'zsh -i -c exit' --warmup 3 --runs 5 --export-json bench.json
Benchmark 1: zsh -i -c exit
  Time (mean ± σ):      30.1 ms ±   0.2 ms    [User: 19.6 ms, System: 9.2 ms]
  Range (min … max):    30.0 ms …  30.4 ms    5 runs

//}

=== Neovim の起動時間の計測
Neovim の起動時間を計測するために、下記のコマンドを使用します。
//list[guts-nvim-command][起動時間を計測するための Neovim のコマンド][bash]{
nvim --headless -c q
//}

--headless で UI を起動しないようにし、-c q で設定ファイルを読んだ後に q コマンドを実行するようにします。

//list[guts-nvim-result][Neovim の起動時間を計測する例][bash]{
$ hyperfine 'nvim --headless -c q' --warmup 3 --runs 5 --export-json bench.json
Benchmark 1: nvim --headless -c q
  Time (mean ± σ):      95.0 ms ±   1.9 ms    [User: 44.0 ms, System: 50.0 ms]
  Range (min … max):    92.1 ms …  97.2 ms    5 runs

//}

== GitHub Actions でコミットごとのベンチマークを取る
前節で、zsh と Neovim の起動時間を計測できるようになりました。
いよいよ GitHub Actions でコミット毎のベンチマークを取っていきましょう。
最終的には @<img>{guts_benchmark_example} に示すようなベンチマークグラフを GitHub Pages に表示します。

//image[guts_benchmark_example][ベンチマークグラフの例]{
//}

=== github-action-benchmark
ベンチマークグラフを GitHub Pages に出力するために、github-action-benchmark @<fn>{guts-github-action-benchmark} を使用します。

GitHub Pages を使用するので、dotfiles を管理しているリポジトリに gh-pages ブランチを作成しておきます。
//list[guts-o][][bash]{
git checkout -b --orphan gh-pages
git commit -m 'Initial commit' --allow-empty
git push origin gh-pages
//}

=== ベンチマーク結果を JSON ファイルとして出力
github-action-benchmark の入力は、JSON ファイルにする必要があります。
ベンチマーク結果を JSON ファイルとして出力するために、@<list>{guts-benchmark-script} に示すシェルスクリプトを用意します。

//list[guts-benchmark-script][benchmark.sh][bash]{
#!/bin/bash

set -eu

usage()
{
    cat <<EOF
Usage:
    $(basename "${0}") <command> <name>
EOF
}

if [ $# -ne 2 ]; then
    usage
    exit 1
fi

command="${1}"
name="${2}"

hyperfine "${command}" \
    --show-output \
    --warmup 3 \
    --runs 10 \
    --export-json bench.json \
    > /dev/null

mean_time=$(cat bench.json | jq '.results[0].mean * 1000 | tostring')

cat<<EOF
[
    {
        "name": "${name}",
        "unit": "millisecond",
        "value": ${mean_time}
    }
]
EOF
//}

@<list>{guts-benchmark-script} のスクリプトの実行例は、@<list>{guts-benchmark-script-result} のようになります。

//list[guts-benchmark-script-result][benchmark.sh の実行例][bash]{
$ ./benchmark.sh 'zsh -i -c exit' 'zsh load time'
[
    {
        "name": "zsh load time",
        "unit": "millisecond",
        "value": "29.77712492"
    }
]
//}

=== GitHub Actions のワークフロー

今回使用する GitHub Actions のワークフローファイルは @<list>{guts-github-workflow} のようになります。

//list[guts-github-workflow][.github/workflows/test.yaml][yaml]{
name: test

on:
  push:
    branches:
      - master

permissions:
  deployments: write
  contents: write

jobs:
  test:
    strategy:
      fail-fast: false
      matrix:
        os: [ubuntu-20.04, macos-12]
    timeout-minutes: 10

    runs-on: ${{ matrix.os }}

    steps:
      - uses: actions/checkout@v2
        with:
          submodules: true
          fetch-depth: 0

      - name: Install test dependencies
        if: runner.os == 'linux'
        run: |
          sudo apt update
          sudo apt install zsh

          # Install hyperfine
          wget https://github.com/sharkdp/hyperfine/releases/download/v1.14.0/hype
rfine_1.14.0_amd64.deb
          sudo dpkg -i hyperfine_1.14.0_amd64.deb

      - name: Install test dependencies
        if: runner.os == 'macos'
        run: brew install zsh hyperfine

      - name: Install dotfiles
        run: make init

      - name: Run zsh benchmark
        run: |
          ./scripts/benchmark.sh 'zsh -i -c exit' 'zsh load time (${{ runner.os }})
' > zsh-benchmark-result.json
          cat zsh-benchmark-result.json

      - name: Run neovim benchmark
        run: |
          ./scripts/benchmark.sh 'nvim --headless -c q' 'nvim load time (${{ runne
r.os }})' > nvim-benchmark-result.json
          cat nvim-benchmark-result.json

      - name: Store zsh benchmark result
        uses: benchmark-action/github-action-benchmark@v1
        with:
          name: Zsh Benchmark
          tool: "customSmallerIsBetter"
          output-file-path: zsh-benchmark-result.json
          github-token: ${{ secrets.GITHUB_TOKEN }}
          auto-push: true

      - name: Store neovim benchmark result
        uses: benchmark-action/github-action-benchmark@v1
        with:
          name: Neovim Benchmark
          tool: "customSmallerIsBetter"
          output-file-path: nvim-benchmark-result.json
          github-token: ${{ secrets.GITHUB_TOKEN }}
          auto-push: true
//}

以下、各ステップの補足です。

//list[guts-github-workflow-install-dotfiles][Install dotfiles][yaml]{
      - name: Install dotfiles
        run: make init
//}
私の場合、このステップで、~/.zshrc や ~/.vim/init.vim といったdotfiles のシンボリックリンクを生成しています。また、同時に Neovim を含むコマンドラインツールのインストールも行っています。

//list[guts-github-workflow-run-benchmark][Run zsh/neovim benchmark][yaml]{
      - name: Run zsh benchmark
        run: |
          ./scripts/benchmark.sh 'zsh -i -c exit' 'zsh load time (${{ runner.os }})
' > zsh-benchmark-result.json
          cat zsh-benchmark-result.json

      - name: Run neovim benchmark
        run: |
          ./scripts/benchmark.sh 'nvim --headless -c q' 'nvim load time (${{ runn
er.os }})' > nvim-benchmark-result.json
          cat nvim-benchmark-result.json
//}
benchmark.sh を使用し、zsh と neovim のそれぞれのベンチマーク結果を JSON として出力しています。各 OS ごとにベンチマーク結果を生成しており、${{ runner.os }} で現在実行中の OS を取得しています。

//list[guts-github-workflow-store-benchmark][Store zsh/neovim benchmark result][yaml]{
      - name: Store zsh benchmark result
        uses: benchmark-action/github-action-benchmark@v1
        with:
          name: Zsh Benchmark
          tool: "customSmallerIsBetter"
          output-file-path: zsh-benchmark-result.json
          github-token: ${{ secrets.GITHUB_TOKEN }}
          auto-push: true

      - name: Store neovim benchmark result
        uses: benchmark-action/github-action-benchmark@v1
        with:
          name: Neovim Benchmark
          tool: "customSmallerIsBetter"
          output-file-path: nvim-benchmark-result.json
          github-token: ${{ secrets.GITHUB_TOKEN }}
          auto-push: true
//}
benchmark.sh で生成したベンチマーク結果の JSON ファイルを github-action-benchmark に渡し、gh-pages にベンチマークグラフを出力します。

=== ベンチマークグラフ
適当にコミットして、GitHub Actions を実行しましょう。
最終的に、@<img>{guts_zsh_benchmark_result}, @<img>{guts_neovim_benchmark_result} のようなベンチマークグラフが gh-pages に表示されます。

//image[guts_zsh_benchmark_result][zsh のベンチマークグラフ]{
//}

//image[guts_neovim_benchmark_result][Neovim のベンチマークグラフ]{
//}

== おわりに
本章では、GitHub Actions でコミット毎に zsh と Neovim のベンチマークを取る方法を紹介しました。
みなさんも、お気に入りのシェルやエディタのベンチマークを取ってみてはいかがでしょうか。
筆者の dotfiles は、@<href>{https://github.com/yuta1402/dotfiles} に置いてありますので、参考にしてみてください。

//footnote[guts-dotfiles][.bashrc や .vimrc といったコマンドラインツールの設定ファイルのこと]
//footnote[guts-hyperfine][@<href>{https://github.com/sharkdp/hyperfine}]
//footnote[guts-github-action-benchmark][@<href>{https://github.com/benchmark-action/github-action-benchmark}]
