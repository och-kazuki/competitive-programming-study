set shell := ["bash", "-euo", "pipefail", "-c"]

root := justfile_directory()

# コマンド一覧を表示する
default:
    @just --list

# compile_flags.txt を作り直す
sync:
    "{{root}}/scripts/sync-dev-files"

# 問題ディレクトリを作る。例: just new abc400 a
new contest problem:
    "{{root}}/scripts/new-problem" "{{contest}}" "{{problem}}"

# C++ をコンパイルして実行する。パスは just を実行したディレクトリ基準。
[no-cd]
runcpp *args:
    "{{root}}/scripts/runcpp" {{args}}

# Python を実行する。パスは just を実行したディレクトリ基準。
[no-cd]
runpy *args:
    "{{root}}/scripts/runpy" {{args}}

# ディレクトリ内の a.cpp を oj でサンプルテストする。既定はデバッグビルド。
[no-cd]
oj-cpp dir=".":
    "{{root}}/scripts/oj-test" cpp "{{dir}}"

# 速度確認用。サニタイザなしの通常ビルドでサンプルテストする。
[no-cd]
oj-cpp-release dir=".":
    CP_CXX_MODE=release "{{root}}/scripts/oj-test" cpp "{{dir}}"

# 通常ビルドで C++ を実行する。
[no-cd]
runcpp-release *args:
    CP_CXX_MODE=release "{{root}}/scripts/runcpp" {{args}}

# ディレクトリ内の a.py を oj でサンプルテストする。
[no-cd]
oj-py dir=".":
    "{{root}}/scripts/oj-test" py "{{dir}}"

# gen と naive を、無いときだけ今のディレクトリへ置く。
[no-cd]
stress-init:
    "{{root}}/scripts/stress-init"

# 本解と愚直解を繰り返して比べる。例: just stress 100
[no-cd]
stress times="100":
    CP_STRESS_COUNT="{{times}}" "{{root}}/scripts/stress"

# 引用符の include を展開して標準出力へ出す。
[no-cd]
bundle src:
    "{{root}}/scripts/require-dev-shell"
    python3 "{{root}}/scripts/bundle_cpp.py" -I "{{root}}" "{{src}}"

# 問題ディレクトリの a.cpp を展開し submit.cpp を書く。
[no-cd]
submit-file dir=".":
    "{{root}}/scripts/submit-file" "{{dir}}"

# 展開した提出コードをクリップボードへコピーする。
[no-cd]
copy target=".":
    "{{root}}/scripts/copy-submit" "{{target}}"

# テンプレート、ライブラリ、ruff、oj、pyright を確認する
check:
    "{{root}}/scripts/check"
    "{{root}}/scripts/oj-smoke"
    "{{root}}/scripts/check-cwd"
    "{{root}}/scripts/check-nix-root"
    "{{root}}/scripts/check-copy"
    "{{root}}/scripts/check-contest"
    "{{root}}/scripts/check-outside"
    pyright
