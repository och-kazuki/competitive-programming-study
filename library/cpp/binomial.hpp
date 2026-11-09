#pragma once

#include "library/cpp/template.hpp"

#include <atcoder/modint.hpp>

// 階乗と逆元を n まで持つ。m が素数の modint で使う。
template <class Mint> struct Binomial {
  vc<Mint> fact;
  vc<Mint> ifact;

  Binomial() { build(0); }

  explicit Binomial(int n) { build(n); }

  void build(int n) {
    fact.assign(n + 1, Mint(1));
    ifact.assign(n + 1, Mint(1));
    for (int i = 1; i <= n; i++) {
      fact[i] = fact[i - 1] * i;
    }
    ifact[n] = fact[n].inv();
    for (int i = n; i >= 1; i--) {
      ifact[i - 1] = ifact[i] * i;
    }
  }

  Mint C(int n, int k) const {
    if (k < 0 || n < 0 || k > n) {
      return Mint(0);
    }
    return fact[n] * ifact[k] * ifact[n - k];
  }

  Mint P(int n, int k) const {
    if (k < 0 || n < 0 || k > n) {
      return Mint(0);
    }
    return fact[n] * ifact[n - k];
  }
};
