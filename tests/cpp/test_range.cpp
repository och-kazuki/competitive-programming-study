#include "library/cpp/range/compress/compress.hpp"
#include "library/cpp/range/cumsum2d/cumsum2d.hpp"
#include "library/cpp/range/imos/imos.hpp"

#include <cstdio>
#include <random>

static int failures = 0;

static void expect(bool ok, const char *msg) {
  if (!ok) {
    std::fprintf(stderr, "fail: %s\n", msg);
    ++failures;
  }
}

static void test_cumsum() {
  vvc<ll> a{{1, 2, 3}, {4, 5, 6}};
  Cumsum2D cs(a);
  expect(cs.sum(0, 0, 2, 3) == 21, "全体");
  expect(cs.sum(0, 1, 2, 3) == 16, "右の列");
  expect(cs.sum(1, 0, 2, 2) == 9, "下の左");
  expect(cs.sum(0, 0, 0, 0) == 0, "空");
  Cumsum2D empty(vvc<ll>{});
  expect(empty.sum(0, 0, 0, 0) == 0, "空の表");
}

static void test_imos() {
  std::mt19937 rng(1);
  for (int trial = 0; trial < 50; trial++) {
    int n = 1 + (int)(rng() % 12);
    Imos1D im(n);
    vi naive(n);
    int q = 20;
    for (int i = 0; i < q; i++) {
      int l = (int)(rng() % (n + 1));
      int r = (int)(rng() % (n + 1));
      if (l > r) {
        std::swap(l, r);
      }
      ll v = (ll)(rng() % 21) - 10;
      im.add(l, r, v);
      for (int x = l; x < r; x++) {
        naive[x] += v;
      }
    }
    expect(im.build() == naive, "1次元いもす");
  }
  for (int trial = 0; trial < 30; trial++) {
    int h = 1 + (int)(rng() % 6);
    int w = 1 + (int)(rng() % 6);
    Imos2D im(h, w);
    vvc<ll> naive(h, vi(w));
    for (int i = 0; i < 15; i++) {
      int y1 = (int)(rng() % (h + 1));
      int y2 = (int)(rng() % (h + 1));
      int x1 = (int)(rng() % (w + 1));
      int x2 = (int)(rng() % (w + 1));
      if (y1 > y2) {
        std::swap(y1, y2);
      }
      if (x1 > x2) {
        std::swap(x1, x2);
      }
      ll v = (ll)(rng() % 11) - 5;
      im.add(y1, x1, y2, x2, v);
      for (int y = y1; y < y2; y++) {
        for (int x = x1; x < x2; x++) {
          naive[y][x] += v;
        }
      }
    }
    expect(im.build() == naive, "2次元いもす");
  }
}

static void test_compress() {
  Compress<ll> cp;
  cp.add(3);
  cp.add(-1);
  cp.add(3);
  cp.add(7);
  cp.add(-1);
  cp.build();
  expect(cp.size() == 3, "種類");
  expect(cp.xs[0] == -1 && cp.xs[1] == 3 && cp.xs[2] == 7, "昇順");
  expect(cp.id(-1) == 0 && cp.id(3) == 1 && cp.id(7) == 2, "順位");
  expect(cp.id(0) == 1, "間の lower_bound");
  Compress<int> built(vc<int>{5, 1, 5, 2});
  expect(built.size() == 3 && built.id(1) == 0 && built.id(5) == 2, "構築");
}

int main() {
  test_cumsum();
  test_imos();
  test_compress();
  if (failures) {
    std::fprintf(stderr, "%d failures\n", failures);
    return 1;
  }
  return 0;
}
