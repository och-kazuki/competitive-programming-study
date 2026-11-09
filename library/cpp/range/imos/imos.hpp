#pragma once

#include "library/cpp/template.hpp"

// 半開区間 [l, r) に v を足し、build で各点の値にする。
struct Imos1D {
  int n = 0;
  vi d;

  explicit Imos1D(int n_) : n(n_), d(n_ + 1) {}

  void add(int l, int r, ll v) {
    d[l] += v;
    d[r] -= v;
  }

  vi build() const {
    vi a(n);
    ll cur = 0;
    for (int i = 0; i < n; i++) {
      cur += d[i];
      a[i] = cur;
    }
    return a;
  }
};

// 半開矩形 [y1, y2) × [x1, x2) に v を足す。
struct Imos2D {
  int h = 0;
  int w = 0;
  vvc<ll> d;

  Imos2D(int h_, int w_) : h(h_), w(w_), d(h_ + 1, vi(w_ + 1)) {}

  void add(int y1, int x1, int y2, int x2, ll v) {
    d[y1][x1] += v;
    d[y1][x2] -= v;
    d[y2][x1] -= v;
    d[y2][x2] += v;
  }

  vvc<ll> build() const {
    vvc<ll> a(h, vi(w));
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        ll cur = d[y][x];
        if (y) {
          cur += a[y - 1][x];
        }
        if (x) {
          cur += a[y][x - 1];
        }
        if (y && x) {
          cur -= a[y - 1][x - 1];
        }
        a[y][x] = cur;
      }
    }
    return a;
  }
};
