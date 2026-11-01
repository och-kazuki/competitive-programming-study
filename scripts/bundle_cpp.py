#!/usr/bin/env python3
"""引用符形式の #include を展開し、提出用の1ファイルを標準出力へ書く。"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path


def parse_quoted_include(line: str) -> str | None:
    stripped = line.strip()
    if not stripped.startswith("#"):
        return None
    body = stripped[1:].lstrip()
    if not body.startswith("include"):
        return None
    rest = body[len("include") :].lstrip()
    if not rest.startswith('"'):
        return None
    end = rest.find('"', 1)
    if end == -1:
        raise ValueError(f"壊れた include です: {line.rstrip()}")
    return rest[1:end]


def resolve_include(name: str, current: Path, include_dirs: list[Path]) -> Path:
    relative = (current.parent / name).resolve()
    if relative.is_file():
        return relative
    for directory in include_dirs:
        candidate = (directory / name).resolve()
        if candidate.is_file():
            return candidate
    raise FileNotFoundError(name)


def expand(path: Path, include_dirs: list[Path], seen: set[Path]) -> str:
    path = path.resolve()
    if path in seen:
        return ""
    seen.add(path)
    chunks: list[str] = []
    for line in path.read_text(encoding="utf-8").splitlines(keepends=True):
        # 提出ファイルでは #pragma once が警告になる。展開時に外す。
        if line.strip() == "#pragma once":
            continue
        name = parse_quoted_include(line)
        if name is None:
            chunks.append(line if line.endswith("\n") else f"{line}\n")
            continue
        included = resolve_include(name, path, include_dirs)
        chunks.append(f"// begin {name}\n")
        nested = expand(included, include_dirs, seen)
        if nested and not nested.endswith("\n"):
            nested += "\n"
        chunks.append(nested)
        chunks.append(f"// end {name}\n")
    return "".join(chunks)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="引用符の include を展開する")
    parser.add_argument("source", type=Path)
    parser.add_argument("-I", dest="includes", action="append", default=[], type=Path)
    args = parser.parse_args(argv)
    root = Path(__file__).resolve().parent.parent
    include_dirs = [path.resolve() for path in args.includes] or [root]
    try:
        text = expand(args.source.resolve(), include_dirs, set())
    except FileNotFoundError as exc:
        print(f"include が見つかりません: {exc}", file=sys.stderr)
        return 1
    sys.stdout.write(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
