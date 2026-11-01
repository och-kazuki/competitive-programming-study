// テンプレートの使用例。問題ごとの解法は含まない。
// ビルド例:
//   scripts/cp-gxx -o /tmp/example library/cpp/example.cpp
//   scripts/cp-gxx -DLOCAL -o /tmp/example library/cpp/example.cpp
#include "library/cpp/template.hpp"

int main() {
  // 宣言マクロは文が二つになる。if の中では波括弧が必要。
  // if (cond) {
  //   INT(n);
  // }
  // 対話問題では print のあと、cout << flush; で出力を送る。

  vi a{3, 1, 2, 1};
  UNIQUE(a);
  print(a);
  print(SUM<ll>(a), LB(a, 2), len(a));

  ll best = INF;
  chmin(best, 10);
  YES(best == 10);

  ll total = 0;
  FOR(i, len(a)) total += a[i];
  FOR_R(i, len(a)) total += a[i];
  print(total);

  SHOW(a, best, total);
  return 0;
}
