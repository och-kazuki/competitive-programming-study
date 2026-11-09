# 二項係数

「`n` 個から `k` 個を選ぶ通り数」を、素数の余りで何度も求めるときに使う。階乗 `n!` と、その逆元を `0` から上限まで先に計算しておき、組み合わせは掛け算3つで出す。

法 `m` が素数なら、`1` から `m` 未満の数には逆元がある。`n!` の逆元が分かれば、`C(n, k) = n! / (k! (n-k)!)` を割り算ではなく逆元の掛け算で計算できる。

## include

```cpp
#include "library/cpp/math/binomial/binomial.hpp"
```

`template.hpp` と `<atcoder/modint.hpp>` を読む。山括弧の AtCoder Library は、提出用に展開しない。

## API

```cpp
template <class Mint>
struct Binomial {
  vc<Mint> fact, ifact;
  Binomial();
  explicit Binomial(int n);
  void build(int n);
  Mint C(int n, int k) const;
  Mint P(int n, int k) const;
};
```

- `Mint` は AtCoder Library の modint。`modint998244353`、`modint1000000007`、`dynamic_modint` を渡せる。
- `Binomial(n)` と `build(n)` は、`0!` から `n!` までと、それぞれの逆元を作る。
- `C(n, k)` は `n` 個から `k` 個を選ぶ数。`k < 0` または `k > n` または `n < 0` なら `0`。
- `P(n, k)` は順列 `n! / (n-k)!`。範囲外は `0`。
- `n` と `k` は `0` 以上で、`build` した上限以下であること。上限を超えた添字は範囲外参照になる。
- 余りは `Mint` の法。`dynamic_modint` は、`build` の前に `set_mod` する。法を変えたら、作り直す。

## 計算量と制約

`build(n)` は `O(n)`。クエリは `O(1)`。`n` が `10^7` なら、`Mint` が 4 バイトとして配列2本で約 80 MB。法は素数であること。`fact[n].inv()` は、法と `n!` が互いに素なときに正しく、素数法で `n < 法` なら満たされる。

## 使用例

`998244353` で `C(5, 2)` を求める。値は `10`。

```cpp
#include "library/cpp/math/binomial/binomial.hpp"

int main() {
  using mint = atcoder::modint998244353;
  Binomial<mint> binom(5);
  print(binom.C(5, 2).val());
  return 0;
}
```

出力:

```text
10
```

## こういう問題なら使う

「組み合わせや順列を `998244353` や `10^9+7` で何度も求める」「`n` が `10^6` や `10^7` まで」。`n` が法より大きい、または法が合成数のときは、この前計算だけでは足りない。`n` が `10^9` で選ぶ個数だけ小さいときは、`k` 回の掛け算で直接書く方が配列を確保しない。

確認できた例題:

- [ABC 132 D - Blue and Red Balls](https://atcoder.jp/contests/abc132/tasks/abc132_d)。青いボールの塊の個数ごとに並べ方を数える。`N` は `2000` 以下、余りは `10^9+7`。ページを取得し、並べ方の数を `10^9+7` で求める問題文であることを確認した。
- [ABC 156 D - Bouquet](https://atcoder.jp/contests/abc156/tasks/abc156_d)。`n` 種類から 1 本以上を選ぶ通り数から、本数が `a` または `b` のものを除く。ページを取得し、組み合わせの余りを求める問題文であることを確認した。`n` は `10^9` までなので、`n!` を `n` まで確保する使い方ではなく、必要な `C(n, k)` だけを掛け算で求める側の問題である。

## 注意点

- `build` の上限より大きい `n` を `C` に渡すと、配列の外を読む。
- `dynamic_modint` で `set_mod` を `build` の後に変えても、入っている値は古い法のままである。法を変えたら `build` をやり直す。
- `n >= 法` の階乗は法で `0` になる。その逆元は作れない。クエリの `n` が法未満であることを問題文で確認する。
- `C(n, k)` の引数が `ll` のままだと、`int` へ狭めるときに溢れた値になることがある。上限が `int` に収まることを見てから渡す。

## 関連

素因数の一覧が欲しいときは [篩](../sieve/README.md)。AtCoder Library の modint が余りと逆元を担当し、このヘッダは階乗の表を持つ。畳み込みや形式的冪級数は AtCoder Library の別ヘッダを使う。
