# competitive-programming-study

競技プログラミングの学習用環境です。Nix で C++ と Python を揃え、提出用テンプレート、コンテスト中の作業ファイル、ライブラリ、ノートを分けて扱います。

対応 OS は Linux と macOS（x86_64 と aarch64）です。

## はじめての1問

解答を書く場所は `work/<コンテスト>/<問題>/` です。次はコンテスト `abc400` の問題 `a` の例です。`nix develop` に入ってから実行します。

```sh
just new abc400 a
```

できるファイルは次のとおりです。

| パス | 役割 |
| --- | --- |
| `work/abc400/a/a.cpp` | C++ の解答。ここだけ編集する |
| `work/abc400/a/a.py` | Python の解答。ここだけ編集する |
| `work/abc400/a/test/` | サンプル入出力を置く場所 |

`a.cpp` は最初から `library/cpp/template.hpp` を include しています。`main` に解答を書きます。テンプレートの関数やマクロはヘッダ側にあり、解答ファイルでは変えません。

`just oj-cpp` のように引数を省略すると、`just` を実行したディレクトリを見ます。相対パスを渡したときも、そのディレクトリからの相対パスです。

```sh
cd work/abc400/a
oj d https://atcoder.jp/contests/abc400/tasks/abc400_a
runcpp
just oj-cpp
just submit-file
```

- `oj d <問題URL>` は、今いるディレクトリの `test/` にサンプルを保存します。
- `runcpp` は、今いるディレクトリの `a.cpp` をコンパイルして実行します。入力はそのまま渡せます。
- `just oj-cpp` は `test/` のサンプルで `a.cpp` を試します。Python なら `runpy` と `just oj-py` です。
- `just submit-file` はヘッダを展開し、`work/abc400/a/submit.cpp` を作ります。これを提出します。
- コンテスト後のメモは `notes/upsolve/abc400-a.md` に書きます。書き方は [notes/upsolve/README.md](notes/upsolve/README.md) です。

VS Code と Cursor では、`a.cpp` または `a.py` を開いた状態でタスクを実行します。開いているファイルを実行するタスクと、そのフォルダの `a.cpp` / `a.py` を実行・テストするタスクがあります。サンプル取得はタスク「oj: サンプルを取得」に問題 URL を渡します。

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

### 1. 問題用のファイルを作る

```sh
just new abc400 a
```

解答は `work/abc400/a/a.cpp` と `work/abc400/a/a.py` です。サンプルは `work/abc400/a/test/` に置きます。このディレクトリは git の管理外です。コンテスト名と問題名には英数字、`_`、`-` が使えます。

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

問題ディレクトリでサンプルを取得します。

```sh
cd work/abc400/a
oj d https://atcoder.jp/contests/abc400/tasks/abc400_a
just oj-cpp
just oj-py
```

`oj d` は `test/sample-1.in` と `test/sample-1.out` のようなファイルを作ります。手で同じ名前のファイルを置いても同じです。タスク「oj: サンプルを取得」は問題 URL を聞いて、開いているファイルのフォルダに保存します。タスク「oj: サンプルテスト (C++)」「oj: サンプルテスト (Python)」はそのフォルダをテストします。

### 4. 提出用ファイルを作る

C++ の解答は `library/cpp/template.hpp` を include しています。提出前に1ファイルへ展開します。

```sh
just submit-file work/abc400/a
```

`work/abc400/a/submit.cpp` ができます。中身は単体でコンパイルできます。`#include <atcoder/...>` は展開しません。AtCoder のジャッジにライブラリがあるためです。AtCoder Library を使うときは `a.cpp` に次を書きます。

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
