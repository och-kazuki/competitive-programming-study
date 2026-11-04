# 二分探索

単調な述語の、true と false の境界を探す。

このノートのコードは、すべてめぐる式で書く。

述語 `f` は、ある境目より手前はすべて同じ真偽で、境目を越えると一度だけ反転する。配列がソート済みなら「`x` 以上か」、答えの候補なら「この値で条件を満たすか」が、その述語になる。

## 不変条件で縮める

境界そのものを直接追わず、次の2点を保ったまま区間を半分にする。

- `ok` は常に `f(ok)` が true
- `ng` は常に `f(ng)` が false

`|ok - ng| > 1` の間だけループする。終わったとき `ok` と `ng` は隣り合い、答えは `ok` 側の端である。

```cpp
#include "library/cpp/template.hpp"

template <class F>
ll boundary(ll ok, ll ng, F f) {
  while (std::llabs(ok - ng) > 1) {
    ll mid = ng + (ok - ng) / 2;
    if (f(mid)) ok = mid;
    else ng = mid;
  }
  return ok;
}
```

`mid` は `ok` と `ng` のあいだに入る。`f(mid)` が true なら `ok` を、false なら `ng` を `mid` に寄せる。どちらに寄せても、上の2つの不変条件は壊れない。

Python も同じ不変条件である。整数は多倍長なので、和は溢れない。

```python
def boundary(ok: int, ng: int, f) -> int:
    while abs(ok - ng) > 1:
        mid = (ok + ng) // 2
        if f(mid):
            ok = mid
        else:
            ng = mid
    return ok
```

`x >= 3` を、整数 `0` から `6` で最小化する例である。初期値は範囲の外に置く（次の節）。

```text
最小化。境目は F F F T T T T

初期
ng                                  ok
-1   0   1   2   3   4   5   6   7
     F   F   F   T   T   T   T

mid = 3 は T なので ok を 3 へ
ng                  ok
-1   0   1   2   3
     F   F   F   T

mid = 1 は F なので ng を 1 へ
mid = 2 は F なので ng を 2 へ

        ng  ok
-1   0   1   2   3
     F   F   F   T

|ok - ng| = 1 で終了。答えは ok = 3
```

| 回 | mid | f(mid) | 更新後の ng | 更新後の ok |
| --- | --- | --- | --- | --- |
| 初期 |  |  | -1 | 7 |
| 1 | 3 | true | -1 | 3 |
| 2 | 1 | false | 1 | 3 |
| 3 | 2 | false | 2 | 3 |

最大化は、初期の `ok` と `ng` を左右入れ替えるだけである。`x * x <= 10` を `0` 以上 `10` 以下で最大化すると、`ok` は `3` に着く。

```text
最大化。境目は T T T T F F ...

初期
ok                                  ng
-1   0   1   2   3   4   5  ...  11
     T   T   T   T   F   F

終了時は ok = 3, ng = 4。答えは ok = 3
```

## 初期値

範囲を `[L, R]`（両端を含む）とする。条件を満たす点が範囲の外にあるかもしれないので、満たす側と満たさない側の初期値は、どちらも範囲の外に置く。

| 向き | 単調性 | ng の初期値 | ok の初期値 | 終了時の ok |
| --- | --- | --- | --- | --- |
| 最小の true | 一度 true になったら以降も true | `L - 1` | `R + 1` | 最小の true。無ければ `R + 1` |
| 最大の true | 一度 false になったら以降も false | `R + 1` | `L - 1` | 最大の true。無ければ `L - 1` |

`L - 1` や `R + 1` では `f` を呼ばない。呼ばれないので、範囲外を「満たす」「満たさない」とみなして不変条件を始めてよい。解が無いときは、`ok` が初期位置のまま残る。

```cpp
ll minimize(ll L, ll R, auto f) { return boundary(R + 1, L - 1, f); }
ll maximize(ll L, ll R, auto f) { return boundary(L - 1, R + 1, f); }
```

上の2行は説明用である。`auto` 引数は、このリポジトリの `gnu++20` でコンパイルできる。関数として残すときは、`boundary` と同じくテンプレートにする。

## ソート済み配列

昇順の配列 `a` では、述語が「この位置の値は `x` 以上か」になる。自分で境界を書かなくても、同じ境界を返す関数がある。

| 欲しい位置 | C++ | Python |
| --- | --- | --- |
| `x` 以上の最初 | `lower_bound` | `bisect_left` |
| `x` より大きい最初 | `upper_bound` | `bisect_right` |

`lower_bound` が返す添字は、めぐる式で `ng = -1`、`ok = N`、`f(i) = a[i] >= x` としたときの `ok` と同じである。`N` は配列の長さである。`-1` と `N` では `f` を呼ばない。`upper_bound` は、同じ初期値で `f(i) = a[i] > x` である。

```cpp
ll first_ge(const vi &a, ll x) {
  return boundary(len(a), -1, [&](ll i) { return a[i] >= x; });
}
```

`len(a)` は要素数を `ll` で返す。

`a = {1, 2, 2, 4}` のとき、位置は次のとおり。

| x | lower_bound | upper_bound | `x` の個数 |
| --- | --- | --- | --- |
| 2 | 1 | 3 | 2 |
| 3 | 3 | 3 | 0 |
| 0 | 0 | 0 | 0 |
| 5 | 4 | 4 | 0 |

個数は、返った反復子の差である。

```cpp
ll count_eq(const vi &a, ll x) {
  return upper_bound(all(a), x) - lower_bound(all(a), x);
}
ll count_lt(const vi &a, ll x) { return lower_bound(all(a), x) - a.begin(); }
ll count_le(const vi &a, ll x) { return upper_bound(all(a), x) - a.begin(); }
ll count_ge(const vi &a, ll x) { return a.end() - lower_bound(all(a), x); }
ll count_gt(const vi &a, ll x) { return a.end() - upper_bound(all(a), x); }
```

`vi` は `vector<ll>` である。`all(a)` は `begin` と `end` を渡す。

```python
from bisect import bisect_left, bisect_right

def count_eq(a: list[int], x: int) -> int:
    return bisect_right(a, x) - bisect_left(a, x)
```

Python の `bisect_left` が C++ の `lower_bound`、`bisect_right` が `upper_bound` に対応する。

## 答えで二分探索

直接は求めにくい値を、判定問題に落とす。「この値なら条件を満たすか」が単調なら、満たす側の端が答えになる。

- 最小値の最大化、最大値の最小化は、その値を `mid` にして判定する。満たすなら `ok`、満たさないなら `ng` へ寄せる。
- 判定が `O(N)` や `O(N log N)` でも、外側は `O(log (値域))` 回しか呼ばない。

整数の最大化は、`ok = L - 1`、`ng = R + 1` を `boundary` に渡す。

```cpp
template <class F>
ll max_ok(ll L, ll R, F f) { return boundary(L - 1, R + 1, f); }
```

## 実数

整数と違い、隣り合う実数は無い。`|ok - ng| > 1` では終わらないので、回数を 100 に固定する。`double` なら 100 回で、実用上の精度は足りる。

最小化も最大化も同じ関数である。入れ替えるのは、初期値の `ok` と `ng` の向きだけである。

```cpp
template <class F>
double boundary_real(double ok, double ng, F f) {
  for (int iter = 0; iter < 100; iter++) {
    double mid = (ok + ng) / 2;
    if (f(mid)) ok = mid;
    else ng = mid;
  }
  return ok;
}
```

一度 true になったら以降も true（最小の true）なら、`ok` を大きい側、`ng` を小さい側に置く。一度 false になったら以降も false（最大の true）なら、`ok` を小さい側、`ng` を大きい側に置く。`f` は初期の `ok` と `ng` では呼ばない。

```cpp
template <class F>
double minimize_real(double L, double R, F f) { return boundary_real(R, L, f); }
template <class F>
double maximize_real(double L, double R, F f) { return boundary_real(L, R, f); }
```

終了条件を `|ok - ng|` の差にすると、`double` で差が縮まなくなり、ループが終わらないことがある。

## 計算量

探索範囲の幅を `W`、配列長を `N`、判定1回を `T` とする。

| 種類 | 計算量 |
| --- | --- |
| ソート済み配列への1回の探索 | `O(log N)` |
| 整数の答え | `O(T log W)` |
| 実数を `K` 回 | `O(TK)` |

配列を毎回ソートすると、探索よりソートが重くなる。ソートは先に1回だけ行う。

## 注意点

- ループが終わらない。`ok` と `ng` の更新を逆にすると、`mid` が動かなくなる。実数で `|ok - ng|` を終了条件にすると、表現できない差で止まる。
- 整数の `(ok + ng) / 2` は、和が `LLONG_MAX`（およそ `9.2 * 10^18`）を超えるときに溢れる。両方がおよそ `4.6 * 10^18` を超えるときがそれにあたる。`ng + (ok - ng) / 2` を使う。ただし `ok - ng` 自体が溢れるほど範囲が広いとき（負から正へ `9.2 * 10^18` を超えてまたぐとき）は使えない。その場合は範囲を狭めるか、`__int128` で中点を計算する。
- C++ の整数除算は 0 へ向かう。`|ok - ng| > 1` のとき、`ng + (ok - ng) / 2` は `ok` でも `ng` でもない。Python の `//` は負の無限大へ向かうので、`(ok + ng) // 2` は C++ と切れ方が違うことがある。
- 単調でない述語に掛けると、不変条件が途中で壊れる。境界の前後で true と false が一度だけ入れ替わることを先に確認する。
- 終了時に返すのは、満たす側の `ok` である。`ng` を返すと 1 だけずれる。範囲は閉区間 `[L, R]` か半開区間 `[L, R)` かを、初期値と一緒に固定する。
- 判定が重いと、`log W` 回でも間に合わない。判定の計算量を先に見積もる。

## 見返し

- 述語は、境界の前後で一度だけ反転するか。
- 最大化なら `ok = L - 1`、`ng = R + 1`。最小化ならその逆か。実数も、満たす側を `ok`、満たさない側を `ng` に置いたか。
- 整数のループは `|ok - ng| > 1` か。実数は 100 回か。
- 整数の `mid` は `ng + (ok - ng) / 2` か。実数は `(ok + ng) / 2` か。Python は `(ok + ng) // 2` か。
- 返しているのは `ok` か。解が無いとき、整数の `ok` は初期位置のまま残るか。
- 配列の `lower_bound` は `ng = -1`、`ok = N`、`f(i) = a[i] >= x` と一致するか。`upper_bound` は `a[i] > x` か。個数は差で取れているか。

## 関連

`library/` に二分探索の実装はまだ無い。

考え方の確認に使える問題は次のとおり。

- ソート済み列への `lower_bound`: [ABC 077 C - Snuke Festival](https://atcoder.jp/contests/abc077/tasks/arc084_a)
- 整数の答え（買える最大の整数）: [ABC 146 C - Buy an Integer](https://atcoder.jp/contests/abc146/tasks/abc146_c)
- 整数の答え（最大値の最小化）: [ABC 023 D - 射撃王](https://atcoder.jp/contests/abc023/tasks/abc023_d)
- 実数の答え: [ABC 034 D - 食塩水](https://atcoder.jp/contests/abc034/tasks/abc034_d)
