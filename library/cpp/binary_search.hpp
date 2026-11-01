#pragma once

#include <cassert>

// 単調な述語 pred について、pred(x) が真になる最小の整数を返す。
// 前提は ng < ok。慣例として ng は偽、ok は真にしておく。
// 範囲内がすべて偽なら、呼び出し時の ok を返す。
template <class Pred>
long long binary_search_min(long long ng, long long ok, Pred pred) {
  assert(ng < ok);
  while (ok - ng > 1) {
    long long mid = ng + (ok - ng) / 2;
    if (pred(mid)) {
      ok = mid;
    } else {
      ng = mid;
    }
  }
  return ok;
}
