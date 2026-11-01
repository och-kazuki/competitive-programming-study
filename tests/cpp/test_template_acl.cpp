#include "library/cpp/template.hpp"
#include <atcoder/all>

int main() {
  atcoder::dsu d(3);
  d.merge(0, 1);
  if (!d.same(0, 1) || d.same(0, 2)) {
    return 1;
  }

  vi a{3, 1, 2};
  sort(all(a));
  if (len(a) != 3 || a[0] != 1) {
    return 1;
  }
  a.eb(4);
  ll total = 0;
  FOR(i, len(a)) total += a[i];
  if (total != 10) {
    return 1;
  }

  atcoder::fenwick_tree<ll> fw(3);
  fw.add(0, 2);
  if (fw.sum(0, 1) != 2) {
    return 1;
  }

  // convolution.hpp は変数名 len を使う。関数マクロ len() と衝突しないことを見る。
  vc<atcoder::modint998244353> x(1, 1), y(1, 1);
  auto z = atcoder::convolution(x, y);
  if (z.size() != 1 || z[0].val() != 1) {
    return 1;
  }
  return 0;
}
