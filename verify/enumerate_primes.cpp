// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/enumerate_primes
#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_primes"

#include "library/cpp/sieve.hpp"

int main() {
  int n, a, b;
  cin >> n >> a >> b;
  vi primes = primes_upto(n);
  vi out;
  for (ll i = 0; (ll)a * i + b < (ll)primes.size(); i++) {
    out.push_back(primes[(int)((ll)a * i + b)]);
  }
  cout << primes.size() << ' ' << out.size() << '\n';
  for (int i = 0; i < (int)out.size(); i++) {
    if (i) {
      cout << ' ';
    }
    cout << out[i];
  }
  cout << '\n';
  return 0;
}
