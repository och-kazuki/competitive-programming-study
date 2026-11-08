# competitive-programming-study

競技プログラミングの学習用環境です。Nix で C++ と Python を揃え、提出用テンプレート、コンテスト中の作業ファイル、ライブラリ、ノートを分けて扱います。

対応 OS は Linux と macOS（x86_64 と aarch64）です。Windows では WSL2 上の Linux で使います。手順は「Windows」を見てください。

## はじめての1問

リポジトリで `nix develop` に入り、その中で進めます。`abc` はシェル関数です。`just` からは今のシェルのディレクトリを変えられないので、`just abc` ではなく `abc` と打ちます。

```sh
nix develop
abc 400
```

ABC 400 の全問題を `work/abc400/` に作り、サンプルを取得して `work/abc400/a` に移動します。問題数は AtCoder の問題一覧から取るので、A から G までも Ex もそのまま揃います。解答は移動先の `a.cpp` に書きます。`a.cpp` は `library/cpp/template.hpp` を include しています。型、入出力、ループ、デバッグの使い方は [library/cpp/README.md](library/cpp/README.md) と [library/cpp/example.cpp](library/cpp/example.cpp) です。

```sh
just oj
just copy
abc 400 b
```

- `just oj` は、今いるディレクトリの `test/` で `a.cpp` を試します。
- `just copy` は展開した提出コードをクリップボードへコピーします。ファイルに残すときは `just submit-file` です。
- `abc 400 b` は B 問題へ移動します。既にサンプルがある問題は取り直さないので、すぐ終わります。
- 終わるとコンテストの URL を出します。例: `https://atcoder.jp/contests/abc400`。問題記号を付けたとき（`abc 400 e` や `contest arc190 a`）は、問題ページの URL も出します。取得済みでサンプルを取り直さないときも出します。
- 既存の `a.cpp` は上書きしません。
- ARC や AGC は `contest arc190`、`arc 190`、`agc 001 c` のようにします。
- ネットにつながらないときや開始前は、その旨を出して、作れたフォルダだけ残します。
- オフラインで1問だけ空のフォルダが欲しいときは `just new abc400 a` です。
- コンテスト後のメモは `notes/upsolve/abc400-a.md` です。書き方は [notes/upsolve/README.md](notes/upsolve/README.md) です。

Zed では `task: spawn` からタスク「コンテストを準備」を選び、`400` や `400 e` を入力します。終わると `a.cpp` を開きます。提出はタスク「提出用コードをコピー」です。AI 機能はプロジェクト設定で無効です。

## セットアップ

### Nix を入れる

Nix の公式手順でインストールします。

<https://nixos.org/download>

Flakes が無効なら、`~/.config/nix/nix.conf` に次を追加してからシェルを開き直します。

```
experimental-features = nix-command flakes
```

### 開発シェルに入る

リポジトリの中なら、どのディレクトリからでも次で入れます。`work/abc400/a` で `nix develop ../../..` としても、`.venv` と `run` はリポジトリのルートを基準にします。

```sh
nix develop
```

初回は `uv sync` が走り、Python の開発依存（online-judge-tools）を `.venv` に入れます。あわせて `compile_flags.txt` を作ります。

- `.venv` は Python 3.12 の仮想環境です。`oj` とストレステストの `gen.py` がこの Python を使います。Zed の basedpyright は `pyproject.toml` の `[tool.basedpyright]` からこのインタプリタを見ます。
- `compile_flags.txt` は clangd 用のコンパイルフラグです。`-std=gnu++20`、`-DLOCAL`、AtCoder Library、このリポジトリへのインクルードパスが入っています。Nix ストアの絶対パスを含むため、git では管理しません。

シェルでは次が使えます。

- C++: `g++`、`clangd`、`clang-format`、AtCoder Library
- Python: `python`、`uv`、`basedpyright`、`ruff`、`oj`
- 操作: `run`、`just`、`abc`

`just` の一覧は `just --list` で見られます。

`nix develop` の外で `just` や `run` などのスクリプトを実行すると、「先に nix develop を実行してください。」と出て止まります。コンパイルもテストも、この開発シェルの `g++` と `python` だけを使います。`abc` は、そのシェルが起動時に読み込む関数です。

### Windows

Nix は Windows ネイティブでは動きません。Windows では WSL2 の Linux にリポジトリを置き、その中で `nix develop` を使います。MSYS2 のシェルでは使いません。

Zed は WSL 側のフォルダを開きます。WSL のターミナルでリポジトリへ移動し、`zed .` とするか、Zed のフォルダ選択で WSL のパスを開いてください。Windows 側のパスのまま開くと、`nix` もタスクも失敗します。macOS での Zed の動作は未検証です。

### エディタ

エディタは Zed です。設定は `.zed/settings.json` と `.zed/tasks.json` にあります。`disable_ai` は `true` なので、このプロジェクトでは Zed の AI 機能を使いません。

開き方は次のとおりです。

1. WSL2 なら、上のとおり WSL 側のフォルダを Zed で開く。
2. Zed のターミナルでリポジトリのルートへ行き、`nix develop` を実行する。`abc` や `just` はこの中で使います。
3. タスクはコマンドパレットの `task: spawn` から選びます。タスク自体は `nix develop -c` で開発シェルに入ってから動くので、ターミナルがまだ開発シェルでなくても実行できます。

C++ の補完は clangd です。`.clangd` は `-std=gnu++20` と `-DLOCAL` を足します。インクルードパスは、開発シェルに入ったときと `just sync`、タスク「環境ファイルを更新」が作る `compile_flags.txt` に入ります。サニタイザと `_GLIBCXX_DEBUG` は `run` のデバッグビルド用で、clangd のフラグには入れていません。clangd の実行ファイルを Nix のものに固定するときは、`lsp.clangd.binary.path` に `scripts/editor-clangd` の絶対パスを書きます。Zed はこの項目に絶対パスを要求します。このスクリプトは `nix develop --command clangd` を起動します。

Python の解析は basedpyright、整形は Ruff です。型チェックは standard です。インタプリタは `.venv/bin/python` です。設定は `pyproject.toml` の `[tool.basedpyright]` と `.zed/settings.json` にあります。生成器の `gen.py` と、`scripts/` の Python もこの設定で見ます。`.venv` は最初の `nix develop` で作られます。

キーバインドの例です。ユーザーの `keymap.json` に足すと、`ctrl-alt-c` でコピーのタスクを実行できます。

```json
{
  "context": "Workspace",
  "bindings": {
    "ctrl-alt-c": ["task::Spawn", { "task_name": "提出用コードをコピー" }]
  }
}
```

### C++ のスニペット

グリッドの 4 近傍・8 近傍の `dx` / `dy` と範囲外判定 `out_grid` は、共通ヘッダではなく Zed のスニペット `snippets/zed/c++.json` にあります。必要な問題の `a.cpp` にだけ展開します。

| prefix | 中身 | 置く場所 |
| --- | --- | --- |
| `grid4` | 4 近傍の `dx[4]` / `dy[4]`。順序は R, L, D, U（右, 左, 下, 上） | `main` の外（include の後）か `main` の中 |
| `grid8` | 8 近傍の `dx[8]` / `dy[8]`。順序は R, DR, D, DL, L, UL, U, UR（右から時計回り） | `main` の外（include の後）か `main` の中 |
| `outgrid` | `inline bool out_grid(int y, int x, int h, int w)`。グリッドの外なら `true` | 関数定義なので必ず `main` の外（include の後） |
| `lambda` | `auto f = [&](auto x) { ... };`。型推論のラムダ | 式を置ける場所。最初のカーソルは名前、次が引数、最後が本体 |

- `x` は列（`+1` で右）、`y` は行（`+1` で下）です。`out_grid` の引数は行 `y`、列 `x`、行数 `h`、列数 `w` の順です。
- `grid4` と `grid8` は同じ名前 `dx` / `dy` を定義するので、1 つのファイルではどちらか片方だけを使います。
- `dx` / `dy` を使う関数が `main` の外にあるときは、`dx` / `dy` も `main` の外に置きます。

Zed はリポジトリ内のスニペットファイルを自動では読みません。Zed のユーザー設定にあるスニペットファイルへ、手で入れてください。スクリプトでユーザー設定を書き換えることはしません。

1. Zed のコマンドパレットで `snippets: configure snippets` を実行し、言語に C++ を選びます。C++ 用のスニペットファイル（`c++.json`）が開きます。Linux と macOS では `~/.config/zed/snippets/` の下です。フォルダは `snippets: open folder` でも開けます。
2. ファイルが空なら、`snippets/zed/c++.json` の中身をそのまま貼ります。
3. すでに自分のスニペットがあるときは、外側の `{ }` の中に `grid4`、`grid8`、`outgrid`、`lambda` の 4 項目だけを足します。既存の項目は消しません。同じ名前の項目があるときは、どちらを残すか決めてから片方の名前を変えます。
4. 保存します。

WSL2 で Windows 版の Zed を使うときは、スニペットファイルは WSL 側ではなく Zed 本体の設定フォルダにあるはずです。その場所は公式ドキュメントで確認できていないため、パスを手で探さずに `snippets: configure snippets` か `snippets: open folder` で開いてください。

使い方です。

1. 置きたい場所で prefix（`grid4` など）を打ちます。補完メニューにスニペットが出たら選んで確定します。
2. メニューが出ないときは `ctrl-space`（`editor: show completions`）で開きます。Vim モードの挿入モードでは `ctrl-x ctrl-o` でも開けます。
3. 確定後、カーソルは本体の最後（`$0` の位置）に移ります。

確定とプレースホルダ移動のキーは、公式ドキュメントのスニペットのページには書かれていません。Zed の既定キーマップでは、補完の確定が `enter`、スニペットの次のプレースホルダが `tab`、前が `shift-tab` です。自分の `keymap.json` で変えている場合はそちらに従います。設定 `snippet_sort_order` を `none` にしていると、スニペットは補完メニューに出ません。

`just check` は `scripts/check-snippets` で、この JSON が読めること、各本体が上の場所でコンパイルできること、方向と `out_grid` の境界、提出用に展開したあともコンパイルできることを確かめます。

## ディレクトリ構成

```text
templates/a.cpp     新しい問題へコピーされる解答ファイル
templates/gen.py    ストレステストの生成器のひな形
work/               コンテスト中の作業場所（git 管理外）
library/cpp/        C++ ヘッダ。共通部分は template.hpp
tests/              ライブラリテストと oj の動作確認
notes/upsolve/      問題ごとのメモ
notes/algorithms/   アルゴリズムごとのメモ
scripts/            run や oj-test などの実装
justfile            日常的に使うコマンド
flake.nix           Nix の devShell
pyproject.toml      Python（uv）
.zed/               Zed の設定とタスク
snippets/zed/       Zed に手で入れる C++ スニペット（c++.json）
.clangd             clangd へ渡す追加フラグ
```

`work/` だけを無視し、`templates/a.cpp` のような追跡したいファイルまで巻き込まないようにしています。

## 日々の使い方

`nix develop` の中で進めます。

### 1. コンテストを用意する

```sh
abc 400
```

`work/abc400/a` に移動します。解答はそこにある `a.cpp` です。サンプルは `test/` に入っています。別の問題へ行くときは `abc 400 b` です。

### 2. 実行する

問題ディレクトリでは、引数なしでその場の `a.cpp` を実行します。

```sh
cd work/abc400/a
run
```

ファイルを直接渡すこともできます。

```sh
run work/abc400/a/a.cpp
```

標準入力はそのままプログラムへ渡ります。Zed では、開いているファイル向けのタスクと、そのフォルダの `a.cpp` 向けのタスクがあります。

### 3. サンプルをテストする

`abc` がサンプルを取ったあと、問題ディレクトリで実行します。

```sh
just oj
```

`just oj` と `run` は、既定でデバッグビルドです。`-DLOCAL`、`-g`、`-O0`、`-ftrapv`、AddressSanitizer、UndefinedBehaviorSanitizer、`_GLIBCXX_DEBUG` を付けます。最適化を切るのは、最適化すると配列外参照が消えてサニタイザに届かないことがあるためです。`SHOW` は標準エラーへ出るので、`oj t` の正解判定には入りません。速度を見るときは通常ビルドにします。

```sh
just oj-release
CP_CXX_MODE=release run
```

サニタイザは Linux と WSL2 で使います。macOS では Nix の g++ が AddressSanitizer をリンクできないことがあり、そのときは `CP_CXX_MODE=release` にします。`just copy` は展開したソースをコピーするだけなので、サニタイザは付きません。

手で足すときは `test/sample-1.in` と `test/sample-1.out` です。Zed のタスク「oj: サンプルテスト」は、開いているファイルのフォルダをテストします。通常ビルドのタスクは「oj: サンプルテスト (通常ビルド)」です。

### ストレステスト

問題ディレクトリに生成器と愚直解を置くと、本解と出力を繰り返し比べられます。生成器は `gen.py` か `gen.cpp` で、引数にシードを受け取り、1ケースを標準出力へ書きます。愚直解は `naive.py` か `naive.cpp` です。生成器と愚直解は、C++ と Python が両方あるときは C++ を使います。本解は `a.cpp` だけです。

```sh
just stress-init
just stress 100
```

`just stress-init` は、無いときだけ `templates/gen.py` と `templates/naive.cpp` を今のディレクトリへコピーします。回数の既定は 100、1回の制限時間は 2 秒です。`CP_STRESS_TIMEOUT=1 just stress 50` のように変えられます。

食い違ったときは `stress/<seed>.in`、`stress/<seed>.naive.out`、`stress/<seed>.sol.out` に入力と両方の出力を残し、愚直解の出力を `test/sample-N.out` として追加します。実行時エラーとタイムアウトでも止まり、入力を残します。愚直解が成功していれば、その入力と出力も `test/` に足すので、あとから `just oj` で同じケースを再実行できます。

C++ の生成器・愚直解・本解は、既定で通常ビルド（`-O2`）です。100 回比べる処理をサニタイザ付きの `-O0` にすると遅くなり、出力の食い違い自体は通常ビルドでも見つかります。未定義動作で落ちる反例を探すときは `CP_CXX_MODE=debug just stress 20` にします。`run` の既定がデバッグでも、`just stress` は通常ビルドのままです。

Zed のタスクは「ストレステスト」と「ストレステストのひな形を置く」です。ストレステストは試行回数をその場で入力します。空欄なら 100 回です。

### 4. 提出する

C++ の解答は `library/cpp/template.hpp` を include しています。提出前に1ファイルへ展開してコピーします。

```sh
cd work/abc400/a
just copy
```

展開したコードをクリップボードへコピーし、「コピーしました（N行）」と表示します。`#include <atcoder/...>` は展開しません。AtCoder のジャッジにライブラリがあるためです。ファイルに残すときは `just submit-file` です。`SHOW` は `-DLOCAL` を付けたときだけ標準エラーへ出ます。提出物にはその分岐が残りますが、ジャッジは `LOCAL` を定義しないのでデバッグ出力は出ません。AtCoder Library を使うときは `a.cpp` に次を書きます。

```cpp
#include <atcoder/dsu>
```

詳細は [library/cpp/README.md](library/cpp/README.md) です。

### 5. ノートを書く

- 問題の振り返りは `notes/upsolve/abc400-a.md` のように置きます。書き方は [notes/upsolve/README.md](notes/upsolve/README.md)、記入例は [notes/upsolve/example.md](notes/upsolve/example.md) です。
- アルゴリズムの説明は `notes/algorithms/` です。書き方は [notes/algorithms/README.md](notes/algorithms/README.md) です。

### 6. 変更を確認する

```sh
just check
```

テンプレートのコンパイル、C++ のライブラリテスト、ストレステスト、Zed の設定とスニペットの検査、`ruff`、`basedpyright`、ローカルサンプルに対する `oj` を実行します。

GitHub Actions では `nix flake check` のあと、同じ devShell で `just check` を実行します。

## ライセンス

[MIT License](LICENSE) です。Copyright (c) 2026 Kazuki Ochiai。

`library/cpp/template.hpp` の先頭にだけ、このリポジトリの URL を書いたコメントがあります。提出用に展開したコードには、その1回だけ残ります。
