# Python ライブラリ

モジュールは `library/python/` に置きます。`nix develop` と `runpy` は、このディレクトリを `PYTHONPATH` に加えます。

```python
from binary_search import binary_search_min
```

AtCoder の Python 提出は1ファイルです。使った関数を提出ファイルへコピーしてください。C++ の `oj-bundle` に相当する展開はしません。

テストは `tests/python/` に置き、`just check` で `unittest` と `ruff` を実行します。
