#!/usr/bin/env python3
"""AtCoder の問題一覧ページから、フォルダ名と問題 URL を取り出す。"""

from __future__ import annotations

import re
import sys
import urllib.error
import urllib.request
from html import unescape

USER_AGENT = "Mozilla/5.0 (compatible; competitive-programming-study/1.0)"
SYMBOL = re.compile(r"(?i:ex|[a-h]|\d+)\Z")


def main(argv: list[str] | None = None) -> int:
    args = list(sys.argv[1:] if argv is None else argv)
    if len(args) != 1:
        print("使い方: fetch-atcoder-tasks <contest-id>", file=sys.stderr)
        return 1
    contest_id = args[0]
    url = f"https://atcoder.jp/contests/{contest_id}/tasks"
    request = urllib.request.Request(url, headers={"User-Agent": USER_AGENT})
    try:
        with urllib.request.urlopen(request, timeout=20) as response:
            html = response.read().decode("utf-8", "replace")
    except urllib.error.HTTPError as exc:
        print(f"問題一覧を取得できませんでした（HTTP {exc.code}）: {url}", file=sys.stderr)
        return 1
    except urllib.error.URLError as exc:
        print(f"問題一覧を取得できませんでした: {exc.reason}", file=sys.stderr)
        return 1

    pattern = re.compile(
        r'<a[^>]+href="(/contests/'
        + re.escape(contest_id)
        + r'/tasks/([^"]+))"[^>]*>\s*([^<]+?)\s*</a>',
        re.IGNORECASE,
    )
    order: list[str] = []
    labels: dict[str, list[str]] = {}
    for match in pattern.finditer(html):
        task_id = match.group(2)
        label = unescape(re.sub(r"\s+", " ", match.group(3))).strip()
        if task_id not in labels:
            labels[task_id] = []
            order.append(task_id)
        labels[task_id].append(label)

    rows: list[tuple[str, str]] = []
    for task_id in order:
        symbol = next((text for text in labels[task_id] if SYMBOL.fullmatch(text)), None)
        if symbol is None:
            continue
        folder = symbol.lower()
        full = f"https://atcoder.jp/contests/{contest_id}/tasks/{task_id}"
        rows.append((folder, full))
    if not rows:
        print(f"問題一覧に問題が見つかりません: {url}", file=sys.stderr)
        return 1
    for folder, full in rows:
        print(f"{folder} {full}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
