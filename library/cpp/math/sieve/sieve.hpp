#pragma once

#include "library/cpp/template.hpp"

// 最小素因数つきの線形篩。n は「この値まで」を含む。
struct LinearSieve {
  int n = 0;
  vi spf;
  vi primes;

  LinearSieve() = default;

  explicit LinearSieve(int n_) : n(n_), spf(n_ + 1) {
    for (int i = 2; i <= n; i++) {
      if (spf[i] == 0) {
        spf[i] = i;
        primes.push_back(i);
      }
      for (int p : primes) {
        ll v = (ll)i * p;
        if (p > spf[i] || v > n) {
          break;
        }
        spf[(int)v] = p;
      }
    }
  }

  bool is_prime(int x) const { return x >= 2 && x <= n && spf[x] == x; }

  // x は 1 以上 n 以下。1 は空。
  vc<pi> factorize(int x) const {
    vc<pi> res;
    while (x > 1) {
      int p = spf[x];
      int c = 0;
      while (x % p == 0) {
        x /= p;
        c++;
      }
      res.push_back({p, c});
    }
    return res;
  }
};

// n 以下の素数を昇順で返す。大きな n でも区間に分けて篩う。
inline vi primes_upto(int n) {
  vi primes;
  if (n < 2) {
    return primes;
  }
  int limit = (int)std::sqrt((long double)n);
  while ((ll)limit * limit < n) {
    limit++;
  }
  vector<char> small(limit + 1, 1);
  for (int i = 2; (ll)i * i <= limit; i++) {
    if (!small[i]) {
      continue;
    }
    for (int j = i * i; j <= limit; j += i) {
      small[j] = 0;
    }
  }
  vi base;
  for (int i = 2; i <= limit; i++) {
    if (small[i]) {
      base.push_back(i);
    }
  }
  constexpr int seg = 1 << 15;
  vector<char> block(seg);
  for (int low = 2; low <= n; low += seg) {
    int high = min(n + 1, low + seg);
    std::fill(block.begin(), block.begin() + (high - low), 1);
    for (int p : base) {
      ll start = max((ll)p * p, ((ll)low + p - 1) / p * p);
      for (ll j = start; j < high; j += p) {
        block[(int)(j - low)] = 0;
      }
    }
    for (int i = low; i < high; i++) {
      if (block[i - low]) {
        primes.push_back(i);
      }
    }
  }
  return primes;
}

inline u64 mul_mod(u64 a, u64 b, u64 mod) {
  return (u64)((__uint128_t)a * b % mod);
}

inline u64 pow_mod(u64 a, u64 e, u64 mod) {
  u64 r = 1;
  while (e) {
    if (e & 1) {
      r = mul_mod(r, a, mod);
    }
    a = mul_mod(a, a, mod);
    e >>= 1;
  }
  return r;
}

// 64 ビット以下では決定的な Miller-Rabin。
inline bool is_prime_u64(u64 n) {
  if (n < 2) {
    return false;
  }
  for (u64 p : {2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull}) {
    if (n % p == 0) {
      return n == p;
    }
  }
  u64 d = n - 1;
  int s = 0;
  while ((d & 1) == 0) {
    d >>= 1;
    s++;
  }
  auto witness = [&](u64 a) {
    if (a % n == 0) {
      return false;
    }
    u64 x = pow_mod(a, d, n);
    if (x == 1 || x == n - 1) {
      return false;
    }
    for (int i = 1; i < s; i++) {
      x = mul_mod(x, x, n);
      if (x == n - 1) {
        return false;
      }
    }
    return true;
  };
  for (u64 a : {2ull, 325ull, 9375ull, 28178ull, 450775ull, 9780504ull, 1795265022ull}) {
    if (witness(a)) {
      return false;
    }
  }
  return true;
}

inline u64 pollard_rho(u64 n) {
  if (n % 2 == 0) {
    return 2;
  }
  static std::mt19937_64 rng(20260308);
  while (true) {
    u64 c = rng() % (n - 1) + 1;
    u64 x = rng() % n;
    u64 y = x;
    u64 prod = 1;
    u64 g = 1;
    auto f = [&](u64 v) { return (mul_mod(v, v, n) + c) % n; };
    for (int len = 1; g == 1; len <<= 1) {
      x = y;
      for (int i = 0; i < len; i++) {
        y = f(y);
      }
      for (int i = 0; i < len && g == 1; i += 128) {
        int lim = min(128, len - i);
        for (int j = 0; j < lim; j++) {
          y = f(y);
          u64 diff = x > y ? x - y : y - x;
          if (diff) {
            prod = mul_mod(prod, diff, n);
          }
        }
        g = std::gcd(prod, n);
      }
    }
    if (g != n) {
      return g;
    }
  }
}

// 素因数を昇順に、{素数, 指数} で返す。1 は空。
inline vc<pair<u64, int>> factorize(u64 n) {
  vc<pair<u64, int>> res;
  if (n <= 1) {
    return res;
  }
  static LinearSieve small(1000000);
  for (int p : small.primes) {
    if ((u64)p * p > n) {
      break;
    }
    if (n % p == 0) {
      int c = 0;
      while (n % p == 0) {
        n /= p;
        c++;
      }
      res.push_back({(u64)p, c});
    }
  }
  auto rec = [&](auto &&self, u64 m) -> void {
    if (m == 1) {
      return;
    }
    if (is_prime_u64(m)) {
      res.push_back({m, 1});
      return;
    }
    u64 d = pollard_rho(m);
    while (d == m) {
      d = pollard_rho(m);
    }
    self(self, d);
    self(self, m / d);
  };
  if (n > 1) {
    rec(rec, n);
  }
  std::sort(res.begin(), res.end());
  vc<pair<u64, int>> merged;
  for (auto [p, c] : res) {
    if (!merged.empty() && merged.back().first == p) {
      merged.back().second += c;
    } else {
      merged.push_back({p, c});
    }
  }
  return merged;
}

inline vc<u64> divisors(u64 n) {
  auto fs = factorize(n);
  vc<u64> ds{1};
  for (auto [p, c] : fs) {
    int sz = (int)ds.size();
    u64 pw = 1;
    for (int i = 0; i < c; i++) {
      pw *= p;
      for (int j = 0; j < sz; j++) {
        ds.push_back(ds[j] * pw);
      }
    }
  }
  std::sort(ds.begin(), ds.end());
  return ds;
}
