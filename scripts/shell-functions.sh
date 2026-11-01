# nix develop の bash が shellHook から source する。
# 外から source されても関数は定義しない。

if [ "${CP_IN_DEV_SHELL:-}" != 1 ]; then
  echo "先に nix develop を実行してください。" >&2
  return 1 2>/dev/null || exit 1
fi

_cp_root() {
  if [ -n "${CP_PROJECT_ROOT:-}" ] && [ -f "${CP_PROJECT_ROOT}/flake.nix" ]; then
    printf '%s\n' "$CP_PROJECT_ROOT"
    return 0
  fi
  printf '%s\n' "$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
}

_cp_contest_go() {
  local root dest status dest_file
  root="$(_cp_root)" || return 1
  dest_file="$(mktemp)"
  status=0
  # 移動先は標準出力ではなく専用ファイルで受け取る。oj のログが混ざっても cd できる。
  CP_CONTEST_DEST_FILE="$dest_file" "$root/scripts/prepare-contest" "$@" || status=$?
  dest="$(tail -n 1 "$dest_file" 2>/dev/null | tr -d '\r' || true)"
  rm -f "$dest_file"
  if [ -n "$dest" ] && [ -d "$dest" ]; then
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
