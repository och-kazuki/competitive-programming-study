#pragma once

#include "library/cpp/template.hpp"

// 半開区間 [y1, y2) × [x1, x2) の和を返す。
struct Cumsum2D {
  int h = 0;
  int w = 0;
  vvc<ll> s;

  Cumsum2D() = default;

  explicit Cumsum2D(const vvc<ll> &a) {
    h = (int)a.size();
    w = h ? (int)a[0].size() : 0;
    s.assign(h + 1, vi(w + 1));
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        s[y + 1][x + 1] = a[y][x] + s[y][x + 1] + s[y + 1][x] - s[y][x];
      }
    }
  }

  ll sum(int y1, int x1, int y2, int x2) const {
    return s[y2][x2] - s[y1][x2] - s[y2][x1] + s[y1][x1];
  }
};
