# C++ ライブラリ

ヘッダは `library/cpp/` に置きます。コンパイル時のインクルードパスにはリポジトリのルートが入っているので、解答ファイルからは次のように読みます。

```cpp
#include "library/cpp/template.hpp"
```

`template.hpp` は、解答で繰り返す型・入出力・ループをまとめた共通ヘッダです。短い使用例は [example.cpp](example.cpp) です。新しいアルゴリズムは同じディレクトリにヘッダを足し、解答側で include します。

```cpp
#include "library/cpp/template.hpp"
#include "library/cpp/your_header.hpp"
```

`#include <atcoder/...>` は AtCoder Library です。`nix develop` がパスを通します。問題ごとに使うときは、このヘッダのあとで `a.cpp` に書きます。`len` や `all` は引数を取るマクロなので、AtCoder Library 内の `int len` のような変数とは衝突しません。

```cpp
#include "library/cpp/template.hpp"
#include <atcoder/dsu>
```

提出前に、引用符の include を1ファイルへ展開してコピーします。

```sh
just copy work/abc400/a
```

ファイルに残すときは `just submit-file work/abc400/a` です。`work/abc400/a/submit.cpp` ができます。標準出力へ出すだけなら `just bundle work/abc400/a/a.cpp` です。`#include <atcoder/...>` は展開せず残します。AtCoder のジャッジには AtCoder Library があるためです。

## 型

`vi` は `vector<ll>` です。`vector<int>` ではありません。

| 名前 | 意味 |
| --- | --- |
| `ll` | `long long` |
| `u32` / `u64` | `uint32_t` / `uint64_t` |
| `pi` | `pair<ll, ll>` |
| `vi` | `vector<ll>` |
| `vc<T>` | `vector<T>` |
| `vvc<T>` | `vector<vector<T>>` |
| `pq_max<T>` | 大きい値が先に出る `priority_queue` |
| `pq_min<T>` | 小さい値が先に出る `priority_queue` |

## 入出力

起動時に `ios::sync_with_stdio(false)` と `cin.tie(nullptr)` を設定します。`cout` の精度は 16 桁です。独自の `fread` / `fwrite` バッファは使いません。

- `read(a, b, ...)` は、宣言済みの変数へ読みます。基本型、文字列、`pair`、`vector`、二次元配列に対応します。配列は確保済みの要素を前から順に読み、長さは変えません。
- `print(a, b, ...)` は空白区切りで書き、末尾で改行します。`print()` は改行だけです。二次元配列は行ごとに改行します。
- `YES(cond)` は `YES` または `NO`、`Yes(cond)` は `Yes` または `No` です。

宣言と入力をまとめるマクロです。呼び出したあとも、その変数を使えます。

```cpp
INT(N, M);
LL(A, B);
STR(S);
CHAR(c);
DBL(x);
VEC(ll, A, N);
VV(ll, G, H, W);
```

マクロは文が二つに広がります。`if` の直後に置くときは波括弧が必要です。

```cpp
if (cond) {
  INT(N);
}
```

`print` はフラッシュしません。対話問題では出力のあと `cout << flush;` してください。

カンマを含む型を `VEC` や `VV` に渡すときは、`pi` のような別名にするか、引数を括弧で囲みます。

## ループ

添字の型は `ll`、範囲は半開区間です。境界はループの前に一度だけ評価します。境界の式に副作用は持たせないでください。`step` は正の値です。

```cpp
FOR(n);             // n 回
FOR(i, n);          // 0 <= i < n
FOR(i, l, r);       // l <= i < r
FOR(i, l, r, step); // 正の step で増加
FOR_R(n);           // n 回の逆順
FOR_R(i, n);        // n - 1 から 0
FOR_R(i, l, r);     // r - 1 から l
```

回数だけの形式は、入れ子にしても内部の変数名が重なりません。`FOR_R` は個数を `ll` にしてから減らすので、空の `vector` の `size()` を渡しても符号なし整数はアンダーフローしません。

## 補助

- `all(x)` は `begin()` と `end()`、`rall(x)` は逆方向です。
- `len(x)` は要素数を `ll` で返します。
- `eb` は `emplace_back` です。
- `chmin(a, b)` / `chmax(a, b)` は更新したら true を返します。
- `SUM<T>(A)` は型 `T` の総和です。
- `LB(A, x)` / `UB(A, x)` は、ソート済み配列での `lower_bound` / `upper_bound` の位置です。
- `UNIQUE(A)` はソートして重複を消します。並び順は変わり、`shrink_to_fit()` は呼びません。
- `inf` は `(1 << 30) - 1` です。`int` に収まる大きい値で、法としては使いません。
- `INF` は `1LL << 60` です。`ll` に収まる大きい値です。

## ライブラリ

共通ヘッダとは別に、次のヘッダを足して使います。詳しい API と使用例は、各フォルダの README です。

### 区間・座標

- [一次元累積和](range/cumsum/README.md): 数列の区間の和を前計算する。
- [二次元累積和](range/cumsum2d/README.md): 長方形領域の和を前計算する。
- [いもす法](range/imos/README.md): 区間や長方形への加算をまとめて各点の値にする。
- [座標圧縮](range/compress/README.md): 大きい座標を、大小関係を保った順位に置き換える。

### 数論

- [篩・素因数分解・約数](math/sieve/README.md): 素数判定、素数列挙、64 ビット整数の素因数分解と約数列挙。
- [二項係数](math/binomial/README.md): 階乗と逆元を前計算し、AtCoder Library の modint で組み合わせを求める。

### グラフ

- [ダイクストラ法](graph/dijkstra/README.md): 重みが 0 以上のグラフの最短路。
- [01-BFS](graph/bfs01/README.md): 重みが 0 か 1 だけのグラフの最短路。

グリッドの `dx` / `dy`（4 近傍・8 近傍）と範囲外判定 `out_grid` は、このヘッダにはありません。Zed のスニペット `grid4`、`grid8`、`outgrid`（`snippets/zed/c++.json`）で、必要な問題の `a.cpp` に展開します。`x` は列、`y` は行です。`out_grid` は `main` の外に置き、`grid4` と `grid8` はどちらか片方だけを使います。入れ方と置く場所はルートの [README](../../README.md) の「C++ のスニペット」です。

## デバッグ

`SHOW(a, b)` は、`LOCAL` を定義してコンパイルしたときだけ、変数名と値、関数名、行番号を標準エラーへ出します。実装は [debug.hpp](debug.hpp) にあり、`template.hpp` が `LOCAL` のときだけ読みます。`pair`、`vector`、二次元配列も表示します。`LOCAL` が無いときは `SHOW` は何もせず、引数も評価しません。提出用に展開すると、`debug.hpp` の中身は `#ifdef LOCAL` の中に入ります。ジャッジは `LOCAL` を定義しないので、その部分はコンパイルされず、デバッグ出力は出ません。

```sh
scripts/cp-gxx -DLOCAL -o /tmp/a work/abc400/a/a.cpp
```

`-std=c++17` を後ろに付けると、その標準で上書きできます。通常の `run` は `gnu++20` です。

```sh
scripts/cp-gxx -std=c++17 -DLOCAL -o /tmp/a work/abc400/a/a.cpp
```

整数論、modint、ハッシュマップ、ビット列挙、128 ビット型、最適化 pragma は、このヘッダには入れていません。

テストは `tests/cpp/` に `*.cpp` を置きます。`main` から終了コード 0 で戻るプログラムにしてください。ファイルが無くても `just check` は通ります。追加したテストはコンパイルして実行されます。
