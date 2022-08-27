
= GitHub Actions でシェルとエディタのベンチマークを取ろう

== はじめに
TODO: ほぼメモ書きなので後で編集する

みなさんは、普段使っているシェルやエディタの起動速度が遅くなってしまった経験はないでしょうか？
私は、普段から CLI で作業をすることが多いです。
CLI で作業する際に、特に使用頻度の高いツールがシェルとエディタです。
私の場合、シェルは zsh、エディタは Neovim を使っています。

本章は、dotfiles を GitHub で管理していることを前提としています。

== hyperfine でコマンドのベンチマークを取る

=== hyperfine とは？
hyperfine @<fn>{guts-hyperfine} は Rust で書かれたコマンドのベンチマークツールです。

=== hyperfine の使い方

hyperfine の基本的な使い方は下記の通りです。

//list[guts-hyperfine-command-basic][hyperfine のコマンド例][bash]{
hyperfine 'sleep 0.3'
//}

オプションで、ウォームアップの回数や実行回数などを指定することもできます。

//list[guts-hyperfine-command-warm-up-and-runs][hyperfine のコマンド例 ( ウォームアップ回数と実行回数を指定 )][bash]{
hyperfine 'sleep 0.3' -warmup 3 --runs 5
//}

@<list>{guts-hyperfine-command-warm-up-and-runs} の実行例は下記のようになります。

//list[guts-hyperfine-result-warm-up-and-runs][hyperfine の実行例 ( ウォームアップ回数と実行回数を指定 )][bash]{
$ hyperfine 'sleep 0.3' --warmup 3 --runs 5
Benchmark 1: sleep 0.3
  Time (mean ± σ):     310.8 ms ±   0.7 ms    [User: 1.4 ms, System: 2.1 ms]
  Range (min … max):   310.0 ms … 312.0 ms    5 runs
//}

実行結果をファイルに出力することもできます。

//list[guts-hyperfine-command-export-json][hyperfine のコマンド例 ( json に結果を出力 )][bash]{
hyperfine 'sleep 0.3' --warmup 3 --runs 5 --export-json bench.json
//}


//list[guts-hyperfine-command-export-json][@<list>{guts-hyperfine-command-export-json} で出力される json ファイルの例][json]{
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

=== zsh の起動時間を計る
zsh の起動時間を計るために、下記のコマンドを使用します。
//list[guts-zsh-command][起動時間を計測するための zsh のコマンド][bash]{
zsh -i -c exit
//}
-i でインタラクティブモードで起動し、-c exit で起動後すぐに exit コマンドを実行するようにします。

//list[guts-zsh-result][][bash]{
$ hyperfine 'zsh -i -c exit' --warmup 3 --runs 5 --export-json bench.json
Benchmark 1: zsh -i -c exit
  Time (mean ± σ):      30.1 ms ±   0.2 ms    [User: 19.6 ms, System: 9.2 ms]
  Range (min … max):    30.0 ms …  30.4 ms    5 runs

//}

=== Neovim の起動時間を計る
Neovim の起動時間を計るために、下記のコマンドを使用します。
//list[guts-nvim-command][起動時間を計測するための neovim のコマンド][bash]{
nvim --headless -c q
//}

--headless で UI を起動しないようにし、-c q で設定ファイルを読んだ後に q コマンドを実行するようにします。

//list[guts-nvim-help][][bash]{
$ hyperfine 'nvim --headless -c q' --warmup 3 --runs 5 --export-json bench.json
Benchmark 1: nvim --headless -c q
  Time (mean ± σ):      95.0 ms ±   1.9 ms    [User: 44.0 ms, System: 50.0 ms]
  Range (min … max):    92.1 ms …  97.2 ms    5 runs

//}

== GitHub Actions でコミットごとのベンチマークを取る
前節で、zsh と Neovim の起動時間が計測できるようになりました。
いよいよ GitHub Actions でコミットごとのベンチマークを取っていきましょう。
最終的に、ベンチマーク結果は、下記のようなグラフを gh-pages に表示します。

//image[guts_benchmark_example][ベンチマークのグラフ]{
//}

=== github-action-benchmark
ベンチマーク結果をグラフとして出力するために、github-action-benchmark @<fn>{guts-github-action-benchmark} を使用します。

gh-pages を使用するので、gh-pages ブランチをリモートリポジトリに作成しておきます。
//list[guts-o][][bash]{
git checkout -b --orphan gh-pages
git commit -m 'Initial commit' --allow-empty
git push origin gh-pages
//}

=== ベンチマーク結果を出力するスクリプト

ベンチマーク結果を JSON として出力するため、下記のようなシェルスクリプトを用意します。

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

実際に上記のスクリプトを実行した結果は、下記のようになります。

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

上記のような JSON ファイルを github-action-benchmark の入力として渡します。

=== GitHub Actions のワークフロー

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
筆者の場合、このステップで、~/.zshrc や ~/.vim/init.vim といったdotfiles のシンボリックリンクを生成しています。また、同時に Neovim のインストールも行っています。

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
benchmark.sh を使用し、zsh と neovim のそれぞれのベンチマーク結果を JSON として出力しています。各 OS ごとのベンチマーク結果を生成しており、${{ runner.os }} で現在実行中の OS を取得しています。

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
benchmark.sh で生成したベンチマーク結果の JSON ファイルを github-action-benchmark に渡し、gh-pages にグラフを出力します。

=== ベンチマークグラフ
適当にコミットして、GitHub Actions を実行しましょう。
最終的に、@<img>{guts_zsh_benchmark_result}, @<img>{guts_neovim_benchmark_result} のようなベンチマークグラフが gh-pages に表示されます。

//image[guts_zsh_benchmark_result][zsh のベンチマークグラフ]{
//}

//image[guts_neovim_benchmark_result][Neovim のベンチマークグラフ]{
//}

== おわりに
TODO: ほぼメモ書きなので後で編集する

私の場合、シェルやエディタに env 系のツールを導入したり、プラグインを入れることでその設定ファイルは日々複雑化しています。
複雑化されたシェルやエディタは、いつの間にか起動速度が遅くなってしまいます。

env 系のツールを入れたり、たくさんのプラグインを入れたりすることで、
anyenv や大量のプラグインによって、シェルやエディタは日々複雑化しています。

使用頻度の高いコマンドはその起動時間を短くすることで、日々の時間に少し余裕ができるのではないでしょうか。

//footnote[guts-hyperfine][https://github.com/sharkdp/hyperfine]
//footnote[guts-github-action-benchmark][https://github.com/benchmark-action/github-action-benchmark]
