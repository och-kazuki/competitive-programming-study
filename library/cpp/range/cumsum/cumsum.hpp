#pragma once

#include "library/cpp/template.hpp"

// 先頭は 0。S[i] は A[0] + ... + A[i - 1]。
template <class T, class A> vc<T> cumsum(const A &a) {
  vc<T> s(std::size(a) + 1);
  for (ll i = 0; i < static_cast<ll>(std::size(a)); ++i) {
    s[i + 1] = s[i] + static_cast<T>(a[i]);
  }
  return s;
}
