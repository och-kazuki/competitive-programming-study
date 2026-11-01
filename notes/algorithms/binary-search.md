# 二分探索（最小の真）

## 概要

単調な条件「ある点から先はずっと真」について、真になる最小の整数を探す。範囲を半分ずつ捨てるので、広い探索範囲でも比較回数は対数回で済む。

## いつ使うか

- 答えの大小が分かっていて、境界の片側だけが条件を満たす
- 「条件を満たす最小値」または、符号を反転させれば「条件を満たす最大値」

連続関数の根を直接求める用途には向かない。ここでは整数の境界だけを扱う。

## 使い方

`ng < ok` で呼ぶ。`pred(x)` は `x` が大きくなるほど、一度真になったら真のままであること。

```cpp
#include "library/cpp/binary_search.hpp"

long long ans = binary_search_min(0, 1'000'000'000LL, [&](long long x) {
  return x * x >= n;
});
```

```python
from binary_search import binary_search_min

ans = binary_search_min(0, 1_000_000_000, lambda x: x * x >= n)
```

## 計算量

比較回数は `O(log(ok - ng))`。`pred` の計算量を `T` とすると全体は `O(T log(ok - ng))`。

## 実装

- C++: [library/cpp/binary_search.hpp](../../library/cpp/binary_search.hpp)
- Python: [library/python/binary_search.py](../../library/python/binary_search.py)
- テスト: [tests/cpp/test_binary_search.cpp](../../tests/cpp/test_binary_search.cpp)、[tests/python/test_binary_search.py](../../tests/python/test_binary_search.py)

中央値は `ng + (ok - ng) / 2` にして、`ok - ng` が正の範囲だけで計算する。

## 注意点

- `ng < ok` を崩すと終わらないか、誤った値になる。Python 実装はここで `ValueError` を送る。C++ 実装は `assert` で止める。
- 呼び出し時点の `ok` が偽でも関数は値を返す。その場合の戻り値は初期の `ok` なので、境界が範囲の外にないかを問題ごとに確認する。
- 条件が単調でないと、この手続きは正しい境界を返さない。
