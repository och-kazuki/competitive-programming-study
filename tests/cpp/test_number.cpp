#include "library/cpp/binomial.hpp"
#include "library/cpp/sieve.hpp"

#include <cstdio>
#include <random>

static int failures = 0;

static void expect(bool ok, const char *msg) {
  if (!ok) {
    std::fprintf(stderr, "fail: %s\n", msg);
    ++failures;
  }
}

static void test_sieve() {
  LinearSieve sv(5000);
  vi primes = primes_upto(5000);
  expect(sv.primes == primes, "区間篩と線形篩");
  expect(sv.is_prime(2) && sv.is_prime(4999) && !sv.is_prime(1) && !sv.is_prime(5000), "素数判定");
  for (int x = 1; x <= 5000; x++) {
    ll prod = 1;
    for (auto [p, c] : sv.factorize(x)) {
      expect(sv.is_prime((int)p), "最小素因数");
      for (int i = 0; i < c; i++) {
        prod *= p;
      }
    }
    expect(prod == x, "小さい素因数分解");
  }
  expect(factorize(1).empty(), "1");
  auto ten = factorize(10);
  expect(ten.size() == 2 && ten[0] == pair<u64, int>{2, 1} && ten[1] == pair<u64, int>{5, 1}, "10");
  auto eight = factorize(8);
  expect(eight.size() == 1 && eight[0] == pair<u64, int>{2, 3}, "8");
  std::mt19937_64 rng(2);
  for (int trial = 0; trial < 80; trial++) {
    u64 x = rng() % 1000000000000ull + 1;
    u64 prod = 1;
    u64 prev = 0;
    for (auto [p, c] : factorize(x)) {
      expect(p > prev && is_prime_u64(p) && c > 0, "大きい素因数");
      prev = p;
      for (int i = 0; i < c; i++) {
        prod *= p;
      }
    }
    expect(prod == x, "大きい素因数分解");
    auto ds = divisors(x);
    u64 expect_cnt = 1;
    for (auto [p, c] : factorize(x)) {
      expect_cnt *= (u64)(c + 1);
    }
    expect(ds.size() == expect_cnt && ds.front() == 1 && ds.back() == x, "約数の個数");
    for (u64 d : ds) {
      expect(x % d == 0, "約数");
    }
  }
  u64 semiprime = 1000000007ull * 1000000009ull;
  u64 prod = 1;
  for (auto [p, c] : factorize(semiprime)) {
    for (int i = 0; i < c; i++) {
      prod *= p;
    }
  }
  expect(prod == semiprime, "大きな半素数");
}

static ll brute_c(int n, int k, int mod) {
  if (k < 0 || k > n) {
    return 0;
  }
  ll ans = 1;
  for (int i = 0; i < k; i++) {
    ans = ans * (n - i) / (i + 1);
  }
  return ans % mod;
}

static void test_binomial() {
  using mint = atcoder::modint998244353;
  Binomial<mint> binom(40);
  for (int n = 0; n <= 40; n++) {
    for (int k = 0; k <= n; k++) {
      expect(binom.C(n, k).val() == brute_c(n, k, 998244353), "C");
      mint perm = 1;
      for (int i = 0; i < k; i++) {
        perm *= n - i;
      }
      expect(binom.P(n, k) == perm, "P");
    }
  }
  expect(binom.C(5, 6) == 0 && binom.C(3, -1) == 0, "範囲外");

  using dyn = atcoder::dynamic_modint<0>;
  dyn::set_mod(17);
  Binomial<dyn> small(16);
  for (int n = 0; n < 17; n++) {
    for (int k = 0; k <= n; k++) {
      expect(small.C(n, k).val() == brute_c(n, k, 17), "mod 17");
    }
  }
}

int main() {
  test_sieve();
  test_binomial();
  if (failures) {
    std::fprintf(stderr, "%d failures\n", failures);
    return 1;
  }
  return 0;
}
