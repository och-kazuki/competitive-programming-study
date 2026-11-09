#pragma once

#include "library/cpp/template.hpp"

// 値を昇順の順位へ写す。build のあと、id は lower_bound の位置を返す。
template <class T> struct Compress {
  vc<T> xs;

  Compress() = default;

  explicit Compress(vc<T> v) : xs(std::move(v)) { build(); }

  void add(T x) { xs.push_back(std::move(x)); }

  void build() {
    std::sort(xs.begin(), xs.end());
    xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
  }

  int size() const { return (int)xs.size(); }

  int id(const T &x) const {
    return (int)(std::lower_bound(xs.begin(), xs.end(), x) - xs.begin());
  }
};
