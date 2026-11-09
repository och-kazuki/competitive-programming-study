// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/factorize
#define PROBLEM "https://judge.yosupo.jp/problem/factorize"

#include "library/cpp/math/sieve/sieve.hpp"

int main() {
  int q;
  cin >> q;
  while (q--) {
    u64 a;
    cin >> a;
    auto fs = factorize(a);
    vi xs;
    for (auto [p, c] : fs) {
      for (int i = 0; i < c; i++) {
        xs.push_back((ll)p);
      }
    }
    cout << xs.size();
    for (ll x : xs) {
      cout << ' ' << x;
    }
    cout << '\n';
  }
  return 0;
}
