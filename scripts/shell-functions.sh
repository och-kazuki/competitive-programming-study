# コンテスト用のシェル関数。bash と zsh の両方で source する。
# nix develop の shellHook から読まれる。direnv は関数を親シェルへ渡せないので、
# 普段のシェルで使うときは ~/.bashrc か ~/.zshrc からこのファイルを source する。

if [ -n "${BASH_VERSION:-}" ]; then
  _CP_SHELL_FILE="${BASH_SOURCE[0]}"
elif [ -n "${ZSH_VERSION:-}" ]; then
  eval '_CP_SHELL_FILE="${(%):-%x}"'
else
  _CP_SHELL_FILE="$0"
fi
_CP_ROOT_FROM_FILE="$(cd "$(dirname "$_CP_SHELL_FILE")/.." && pwd)"

_cp_root() {
  if [ -n "${CP_PROJECT_ROOT:-}" ] && [ -f "${CP_PROJECT_ROOT}/flake.nix" ]; then
    printf '%s\n' "$CP_PROJECT_ROOT"
    return 0
  fi
  printf '%s\n' "$_CP_ROOT_FROM_FILE"
}

_cp_contest_go() {
  local root dest status
  root="$(_cp_root)" || return 1
  status=0
  dest="$("$root/scripts/prepare-contest" "$@")" || status=$?
  if [ -n "$dest" ]; then
    cd "$dest" || return 1
  fi
  return "$status"
}

abc() { _cp_contest_go abc "$@"; }
arc() { _cp_contest_go arc "$@"; }
agc() { _cp_contest_go agc "$@"; }
ahc() { _cp_contest_go ahc "$@"; }

contest() {
  if [ "$#" -lt 1 ]; then
    echo "使い方: contest abc 400    または  contest arc190" >&2
    return 1
  fi
  _cp_contest_go "$@"
}
