# 篩・素因数分解・約数

素数かどうか、`n` 以下の素数の一覧、整数の素因数、約数の一覧が欲しいときに使う。

`LinearSieve` は、2 から `n` までを1回ずつ素因数で割る線形篩である。各整数に最小の素因数を覚えておくので、`n` 以下の数は、その素因数を辿って分解できる。`primes_upto` は上限が大きいときに、区間へ分けてエラトステネスの篩を掛ける。`factorize` は 64 ビット整数を分解する。小さい素因数は篩で割り、残った大きい合成数は Pollard のロー法で割る。

## include

```cpp
#include "library/cpp/math/sieve/sieve.hpp"
```

`template.hpp` は、このヘッダが読み込む。

## API

```cpp
struct LinearSieve {
  int n;
  vi spf;
  vi primes;
  explicit LinearSieve(int n);
  bool is_prime(int x) const;
  vc<pi> factorize(int x) const;
};

vi primes_upto(int n);
bool is_prime_u64(u64 n);
vc<pair<u64, int>> factorize(u64 n);
vc<u64> divisors(u64 n);
```

- `LinearSieve(n)` は `0` から `n` までを扱う。`n` を含む。`spf[x]` は `x` の最小素因数。素数なら `spf[x] == x`。`primes` は `n` 以下の素数を昇順に並べたもの。
- `is_prime(x)` は `2 <= x <= n` かつ素数のときだけ真。
- `LinearSieve::factorize(x)` は `1 <= x <= n`。結果は `{素数, 指数}` の昇順。`1` は空。
- `primes_upto(n)` は `n` 以下の素数を昇順で返す。`n < 2` は空。
- `is_prime_u64` は `0` から `2^64 - 1` まで。`2` 未満は偽。
- `factorize` は `0` と `1` は空。それ以外は `{素数, 指数}` の昇順。
- `divisors` は正の約数を昇順で返す。`1` の約数は `{1}`。

## 計算量と制約

`LinearSieve(n)` は時間 `O(n)`、メモリ `O(n)`。`n` が `10^7` 程度ならそのまま使える。`10^8` を超えて最小素因数の配列を持つと、`int` で約 400 MB 以上になる。

`primes_upto(n)` は時間 `O(n log log n)`、追加メモリは `O(sqrt(n))` と区間の大きさ。`n = 5 * 10^8` でも、素数の本数分の配列は必要になる。

`factorize` は、先に `10^6` までの素数で試し割りする。残った数は、素数なら Miller-Rabin、合成数なら Pollard のロー法。`10^18` 以下の 1 個なら通常はすぐ終わる。約数の個数は、`10^18` 以下では数千程度に収まる。

## 使用例

`12` を素因数分解する。`12 = 2^2 * 3`。

```cpp
#include "library/cpp/math/sieve/sieve.hpp"

int main() {
  for (auto [p, c] : factorize(12)) {
    print(p, c);
  }
  return 0;
}
```

出力:

```text
2 2
3 1
```

## こういう問題なら使う

「`X` 以上の最小の素数」「`10^5` 以下の数が素数か、を大量に判定する」「`10^12` 以下の数の素因数の種類数」「`N` 以下の約数の個数の合計」。1個だけが素数かを見るなら、`sqrt(X)` までの試し割りでも足りることが多い。同じ上限で何度も判定するなら篩の方が速い。

確認できた例題:

- [ABC 149 C - Next Prime](https://atcoder.jp/contests/abc149/tasks/abc149_c)。`X` 以上の最小の素数。`X` は `10^5` 以下。ページを取得し、素数の定義と「`X` 以上の最小の素数」という問題文であることを確認した。
- [ABC 084 D - 2017-like Number](https://atcoder.jp/contests/abc084/tasks/abc084_d)。`10^5` 以下で、`N` も `(N+1)/2` も素数である奇数を数える。ページを取得し、その問題文であることを確認した。
- [ABC 142 D - Disjoint Set of Common Divisors](https://atcoder.jp/contests/abc142/tasks/abc142_d)。`10^12` 以下の2数の公約数から、互いに素な組をできるだけ多く選ぶ。ページを取得し、公約数と互いに素という問題文であることを確認した。`gcd` を `factorize` する使い方になる。
- [ABC 172 D - Sum of Divisors](https://atcoder.jp/contests/abc172/tasks/abc172_d)。`N` は `10^7` 以下で、約数の個数を全数について足す。ページを取得し、約数の個数 `f(K)` の総和という問題文であることを確認した。この上限なら `LinearSieve` か、倍数を足す篩が使える。

## 注意点

- `LinearSieve::factorize` に `n` より大きい数を渡さない。64 ビットの分解は、篩オブジェクトではなく関数 `factorize` を使う。
- `factorize(1)` は空である。約数が欲しいときは `divisors(1)` が `{1}` を返す。
- 素因数の積を `int` で持つと溢れる。`10^12` や `10^18` は `u64` のまま扱う。
- `primes_upto` の返り値は素数の一覧なので、`n = 5 * 10^8` では数千万個の `int` になる。メモリを見てから呼ぶ。
- Pollard のロー法は乱数を使う。答えの素因数と指数は一意なので、呼び出しごとに並びは同じになる。

## 関連

素数を法とする組み合わせは [二項係数](../binomial/README.md)。AtCoder Library には、この篩と素因数分解はない。
