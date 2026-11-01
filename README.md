# competitive-programming-study

競技プログラミングの学習用環境です。Nix で C++ と Python を揃え、提出用テンプレート、コンテスト中の作業ファイル、ライブラリ、ノートを分けて扱います。

対応 OS は Linux と macOS（x86_64 と aarch64）です。

## セットアップ

### Nix を入れる

Nix の公式手順でインストールします。

<https://nixos.org/download>

Flakes が無効なら、`~/.config/nix/nix.conf` に次を追加してからシェルを開き直します。

```
experimental-features = nix-command flakes
```

### 開発シェルに入る

リポジトリのルートで次を実行します。

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
templates/          提出用テンプレート（a.cpp と a.py）
work/               コンテスト中の作業場所（git 管理外）
library/cpp/        C++ ヘッダ
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

`work/abc400/a/` に `a.cpp`、`a.py`、空の `test/` ができます。ここは git の管理外です。コンテスト名と問題名には英数字、`_`、`-` が使えます。

### 2. 実行する

問題ディレクトリでは、引数なしでその場のファイルをコンパイルして実行します。

```sh
cd work/abc400/a
runcpp
runpy
```

カレントに `a.cpp` / `a.py` が無いときは `templates/` のテンプレートを実行します。ファイルを直接渡すこともできます。

```sh
runcpp work/abc400/a/a.cpp
runpy work/abc400/a/a.py
```

`just runcpp` と `just runpy` も同じスクリプトです。標準入力はそのままプログラムへ渡ります。

エディタではタスク「C++: ビルドして実行」「Python: 実行」が、開いているファイルを対象にします。

### 3. サンプルをテストする

問題ディレクトリでサンプルを置きます。取得できる問題なら `oj d` を使います。

```sh
cd work/abc400/a
oj d https://atcoder.jp/contests/abc400/tasks/abc400_a
just oj-cpp
just oj-py
```

手で置く場合は `test/sample-1.in` と `test/sample-1.out` です。タスク「oj: サンプルテスト (C++)」「oj: サンプルテスト (Python)」は、開いているファイルのディレクトリをテストします。

### 4. ライブラリを使って提出する

C++ ではリポジトリルートがインクルードパスに入っています。

```cpp
#include "library/cpp/binary_search.hpp"
```

AtCoder Library は次の形です。

```cpp
#include <atcoder/dsu>
```

提出前に、引用符の include を1ファイルへ展開します。

```sh
just bundle work/abc400/a/a.cpp > /tmp/submit.cpp
```

`#include <atcoder/...>` は展開しません。AtCoder のジャッジにライブラリがあるためです。PyPI の online-judge-tools には `oj-bundle` が無いので、同じ役割の展開を `just bundle` で行います。詳細は [library/cpp/README.md](library/cpp/README.md) を見てください。

Python は `library/python` を import できます。

```python
from binary_search import binary_search_min
```

Python の提出は1ファイルなので、使った関数を提出ファイルへコピーします。

### 5. ノートを書く

- 問題の振り返りは `notes/upsolve/`。書き方は [notes/upsolve/README.md](notes/upsolve/README.md)、記入例は [notes/upsolve/example.md](notes/upsolve/example.md)。
- アルゴリズムの説明は `notes/algorithms/`。書き方は [notes/algorithms/README.md](notes/algorithms/README.md)、記入例は [notes/algorithms/binary-search.md](notes/algorithms/binary-search.md)。

### 6. 変更を確認する

```sh
just check
```

テンプレートのコンパイル、C++ と Python のライブラリテスト、`ruff`、`pyright`、ローカルサンプルに対する `oj` を実行します。

GitHub Actions では `nix flake check` のあと、同じ devShell で `just check` を実行します。

## ライセンス

[MIT License](LICENSE) です。Copyright (c) 2026 Kazuki Ochiai。
