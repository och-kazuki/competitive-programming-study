# Python ライブラリ

モジュールは `library/python/` に置きます。`nix develop` と `runpy` は、このディレクトリを `PYTHONPATH` に加えます。

```python
from your_module import solve
```

AtCoder の Python 提出は1ファイルです。使った関数を提出ファイルへコピーしてください。

テストは `tests/python/test_*.py` に置きます。`just check` が `unittest` で実行します。ファイルが無くてもチェックは通ります。
