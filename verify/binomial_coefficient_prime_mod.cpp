// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod
#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"

#include "library/cpp/binomial.hpp"

int main() {
  int t, m;
  cin >> t >> m;
  using mint = atcoder::dynamic_modint<0>;
  mint::set_mod(m);
  int lim = min(m, 10000000) - 1;
  if (lim < 0) {
    lim = 0;
  }
  Binomial<mint> binom(lim);
  while (t--) {
    int n, k;
    cin >> n >> k;
    cout << binom.C(n, k).val() << '\n';
  }
  return 0;
}
