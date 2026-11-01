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

# C++ をコンパイルして実行する
runcpp *args:
    "{{root}}/scripts/runcpp" {{args}}

# Python を実行する
runpy *args:
    "{{root}}/scripts/runpy" {{args}}

# ディレクトリ内の a.cpp を oj でサンプルテストする
oj-cpp dir=".":
    "{{root}}/scripts/oj-test" cpp "{{dir}}"

# ディレクトリ内の a.py を oj でサンプルテストする
oj-py dir=".":
    "{{root}}/scripts/oj-test" py "{{dir}}"

# 引用符の include を展開して標準出力へ出す
bundle src:
    python3 "{{root}}/scripts/bundle_cpp.py" -I "{{root}}" "{{src}}"

# テンプレート、ライブラリ、ruff、oj、pyright を確認する
check:
    "{{root}}/scripts/check"
    "{{root}}/scripts/oj-smoke"
    pyright
