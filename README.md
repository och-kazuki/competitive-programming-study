# competitive-programming-study

競技プログラミングの学習用環境です。Nix で C++ と Python を揃え、提出用テンプレート、コンテスト中の作業ファイル、ライブラリ、ノートを分けて扱います。

対応 OS は Linux と macOS（x86_64 と aarch64）です。

## はじめての1問

`abc` はシェル関数です。`just` からは今のシェルのディレクトリを変えられないので、`just abc` ではなく `abc` と打ちます。`nix develop` に入ると使えます。

```sh
abc 400
```

ABC 400 の全問題を `work/abc400/` に作り、サンプルを取得して `work/abc400/a` に移動します。問題数は AtCoder の問題一覧から取るので、A から G までも Ex もそのまま揃います。解答は移動先の `a.cpp`（Python なら `a.py`）に書きます。`a.cpp` は `library/cpp/template.hpp` を include しています。

```sh
just oj-cpp
just copy
abc 400 b
```

- `just oj-cpp` は、今いるディレクトリの `test/` で `a.cpp` を試します。Python なら `just oj-py` です。
- `just copy` は展開した提出コードをクリップボードへコピーします。ファイルに残すときは `just submit-file` です。
- `abc 400 b` は B 問題へ移動します。既にサンプルがある問題は取り直さないので、すぐ終わります。
- 既存の `a.cpp` は上書きしません。
- ARC や AGC は `contest arc190`、`arc 190`、`agc 001 c` のようにします。
- ネットにつながらないときや開始前は、その旨を出して、作れたフォルダだけ残します。
- オフラインで1問だけ空のフォルダが欲しいときは `just new abc400 a` です。
- コンテスト後のメモは `notes/upsolve/abc400-a.md` です。書き方は [notes/upsolve/README.md](notes/upsolve/README.md) です。

VS Code と Cursor では、タスク「コンテストを準備」に `400` や `400 e` を入れます。終わると `a.cpp` を開きます。提出は、開いているファイルに対するタスク「C++: 提出用コードをコピー」です。

キーバインドの例です。ユーザー設定の `keybindings.json` に足すと、`Ctrl+Alt+C` でコピーできます。

```json
{
  "key": "ctrl+alt+c",
  "command": "workbench.action.tasks.runTask",
  "args": "C++: 提出用コードをコピー"
}
```

## セットアップ

### Nix を入れる

Nix の公式手順でインストールします。

<https://nixos.org/download>

Flakes が無効なら、`~/.config/nix/nix.conf` に次を追加してからシェルを開き直します。

```
experimental-features = nix-command flakes
```

### 開発シェルに入る

リポジトリの中なら、どのディレクトリからでも次で入れます。`work/abc400/a` で `nix develop ../../..` としても、`.venv` と `runcpp` はリポジトリのルートを基準にします。

```sh
nix develop
```

初回は `uv sync` が走り、Python の開発依存（online-judge-tools）を `.venv` に入れます。あわせて `compile_flags.txt` を作ります。

- `.venv` は Python 3.12 の仮想環境です。VS Code、Cursor、そのほかのエディタから同じインタプリタを選べます。
- `compile_flags.txt` は clangd 用のコンパイルフラグです。AtCoder Library とこのリポジトリへのインクルードパスが入っています。Nix ストアの絶対パスを含むため、git では管理しません。

シェルでは次が使えます。

- C++: `g++`、`clangd`、`clang-format`、AtCoder Library
- Python: `python`、`uv`、`pyright`、`ruff`、`oj`
- 操作: `runcpp`、`runpy`、`just`

`just` の一覧は `just --list` で見られます。

### direnv とエディタ

ルートの `.envrc` は `use flake` です。direnv を入れ、シェルの rc に hook を書いたあと、リポジトリで許可します。

```sh
direnv allow
```

`use flake` が使えない古い direnv なら、Nix で新しい direnv を入れます。

```sh
nix profile install nixpkgs#direnv
```

その後、使っているシェルに `eval "$(direnv hook bash)"`（zsh なら `zsh`）を追加します。

`abc` はシェル関数です。direnv は関数を親シェルへ渡せないので、普段の bash（MSYS2 も含む）や zsh で使うときは、設定ファイルに次の1行を足します。パスはクローンした場所に合わせます。`nix develop` の中では shellHook が同じファイルを読むので、この行は無くても動きます。

```sh
source /path/to/competitive-programming-study/scripts/shell-functions.sh
```

Visual Studio Code と Cursor は `.vscode/` を読みます。推奨拡張機能は clangd、Python、Pyright、Ruff、direnv、Just です。direnv 拡張機能を有効にすると、統合ターミナルとタスクが `nix develop` と同じ環境になります。Cursor もこの設定を使います。

clangd は `compile_flags.txt` を、Python は `.venv` と `pyproject.toml` の `[tool.pyright]` を見ます。言語サーバの起動方法はエディタ側の設定に任せ、このリポジトリには特定エディタ専用の init は置いていません。Neovim などを使う場合も、devShell に入った状態で clangd と pyright を自分の LSP から起動すれば同じ補完が使えます。

`compile_flags.txt` がまだ無いときは、タスク「環境ファイルを更新」か `just sync` を実行します。

## ディレクトリ構成

```text
templates/a.cpp     新しい問題へコピーされる C++ の解答ファイル
templates/a.py      新しい問題へコピーされる Python の解答ファイル
work/               コンテスト中の作業場所（git 管理外）
library/cpp/        C++ ヘッダ。共通部分は template.hpp
library/python/     Python モジュール
tests/              ライブラリテストと oj の動作確認
notes/upsolve/      問題ごとのメモ
notes/algorithms/   アルゴリズムごとのメモ
scripts/            runcpp や runpy などの実装
justfile            日常的に使うコマンド
flake.nix           Nix の devShell
pyproject.toml      Python（uv）
.vscode/            VS Code / Cursor の設定
```

`work/` だけを無視し、`templates/a.cpp` のような追跡したいファイルまで巻き込まないようにしています。

## 日々の使い方

開発シェルに入っているか、direnv が有効な状態で進めます。

### 1. コンテストを用意する

```sh
abc 400
```

`work/abc400/a` に移動します。解答はそこにある `a.cpp` と `a.py` です。サンプルは `test/` に入っています。別の問題へ行くときは `abc 400 b` です。

### 2. 実行する

問題ディレクトリでは、引数なしでその場の `a.cpp` / `a.py` を実行します。

```sh
cd work/abc400/a
runcpp
runpy
```

ファイルを直接渡すこともできます。

```sh
runcpp work/abc400/a/a.cpp
runpy work/abc400/a/a.py
```

標準入力はそのままプログラムへ渡ります。エディタでは、開いているファイル向けのタスクと、そのフォルダの `a.cpp` / `a.py` 向けのタスクがあります。

### 3. サンプルをテストする

`abc` がサンプルを取ったあと、問題ディレクトリで実行します。

```sh
just oj-cpp
just oj-py
```

手で足すときは `test/sample-1.in` と `test/sample-1.out` です。タスク「oj: サンプルテスト (C++)」「oj: サンプルテスト (Python)」は、開いているファイルのフォルダをテストします。

### 4. 提出する

C++ の解答は `library/cpp/template.hpp` を include しています。提出前に1ファイルへ展開してコピーします。

```sh
cd work/abc400/a
just copy
```

展開結果がコンパイルできるときだけクリップボードを書き換え、「コピーしました（N行）」と表示します。失敗したときはクリップボードを変えません。`#include <atcoder/...>` は展開しません。AtCoder のジャッジにライブラリがあるためです。ファイルに残すときは `just submit-file` です。AtCoder Library を使うときは `a.cpp` に次を書きます。

```cpp
#include <atcoder/dsu>
```

詳細は [library/cpp/README.md](library/cpp/README.md) です。Python は1ファイルで提出するので、使った関数を `a.py` にコピーします。追加モジュールの置き方は [library/python/README.md](library/python/README.md) です。

### 5. ノートを書く

- 問題の振り返りは `notes/upsolve/abc400-a.md` のように置きます。書き方は [notes/upsolve/README.md](notes/upsolve/README.md)、記入例は [notes/upsolve/example.md](notes/upsolve/example.md) です。
- アルゴリズムの説明は `notes/algorithms/` です。書き方は [notes/algorithms/README.md](notes/algorithms/README.md) です。

### 6. 変更を確認する

```sh
just check
```

テンプレートのコンパイル、C++ と Python のライブラリテスト、`ruff`、`pyright`、ローカルサンプルに対する `oj` を実行します。

GitHub Actions では `nix flake check` のあと、同じ devShell で `just check` を実行します。

## ライセンス

[MIT License](LICENSE) です。Copyright (c) 2026 Kazuki Ochiai。
