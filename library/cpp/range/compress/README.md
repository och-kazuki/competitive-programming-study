# 座標圧縮

出てくる座標 `A_i` の値そのものは大きいが、大小関係だけが欲しいときに使う。出現した値を昇順に並べて重複を消し、各 `A_i` を「小さい方から何番目か」に置き換える。同じ値は同じ順位になる。

## include

```cpp
#include "library/cpp/range/compress/compress.hpp"
```

`template.hpp` は、このヘッダが読み込む。

## API

```cpp
template <class T>
struct Compress {
  vc<T> xs;
  Compress();
  explicit Compress(vc<T> v);
  void add(T x);
  void build();
  int size() const;
  int id(const T &x) const;
};
```

- コンストラクタに配列を渡すと、その場で `build` する。
- `add` は値を追加するだけである。並べ替えは `build` で行う。`add` のあとに `build` を忘れると、`id` は未整理の配列を見る。
- `build` は昇順にソートし、重複を消す。
- `size` は、重複を消したあとの個数。
- `id(x)` は、`xs` の中で `x` 以上である最初の位置。添字は 0 始まり。`x` が登録されていれば、その順位になる。登録されていない値を渡すと、入るべき位置（`lower_bound`）を返す。`x` より大きい値が無ければ `size()` を返す。
- `xs[i]` が、順位 `i` の元の値である。

## 計算量と制約

`build` は `O(N log N)`、各 `id` は `O(log N)`。`N` は値の個数。比較できる型なら使える。

## 使用例

`N` 個の値 `A_i` を読み、0 始まりの順位を 1 行ずつ出す。

入力形式:

```text
N
A_1
...
A_N
```

```cpp
#include "library/cpp/range/compress/compress.hpp"

int main() {
  int N;
  cin >> N;
  vc<ll> A(N);
  for (int i = 0; i < N; i++) {
    cin >> A[i];
  }
  Compress<ll> cp(A);
  for (ll value : A) {
    print(cp.id(value));
  }
  return 0;
}
```

入力例:

```text
3
3
1
3
```

出力例:

```text
1
0
1
```

## こういう問題なら使う

「座標は `10^9` までだが、点や線の個数は `10^5` 程度」「行や列を詰めて、空の行を消したあとの位置を求める」。値の差や実際の距離が必要なときは、順位に置き換えると情報が消える。大小関係や、出現した座標だけを格子にしたいときに使う。

確認できた例題:

- [ABC 036 C - 座圧](https://atcoder.jp/contests/abc036/tasks/abc036_c)。大小関係を保ったまま、非負整数の最大値が最小になるように圧縮する。ページを取得し、座圧そのものを求める問題文であることを確認した。この問題の出力は 0 始まりの順位と一致する。
- [ABC 213 C - Reorder Cards](https://atcoder.jp/contests/abc213/tasks/abc213_c)。最大 `10^9` の行・列から、カードが無い行と列を詰める。ページを取得し、行番号と列番号をそれぞれ圧縮する問題文であることを確認した。問題の行・列は 1 始まりなので、出力は `id + 1` になる。

## 注意点

- `build` の前の `id` は、ソートも重複除去もしていない。
- 同じ値を何度 `add` しても、`build` 後の順位は1つになる。
- 順位は 0 始まりである。問題の出力が 1 始まりなら `id(x) + 1` を出す。
- 元の座標の差が答えに必要な問題では、圧縮した添字だけで距離を計算しない。`xs[id]` に元の値を残してある。

## 関連

圧縮した座標の上で長方形に加算するなら [いもす法](../imos/README.md)、範囲の和を聞くなら [二次元累積和](../cumsum2d/README.md)。`template.hpp` の `UNIQUE` は、その配列をソートして重複を消すだけで、元の位置の順位は覚えない。
