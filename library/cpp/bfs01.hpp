#pragma once

#include "library/cpp/template.hpp"

// 辺の重みは 0 か 1。届かない頂点は INF。
inline vi bfs01(const vc<vc<pi>> &g, int s) {
  int n = (int)g.size();
  vi dist(n, INF);
  deque<int> dq;
  dist[s] = 0;
  dq.push_back(s);
  while (!dq.empty()) {
    int v = dq.front();
    dq.pop_front();
    for (auto [to, w] : g[v]) {
      ll nd = dist[v] + w;
      if (nd >= dist[to]) {
        continue;
      }
      dist[to] = nd;
      if (w == 0) {
        dq.push_front(to);
      } else {
        dq.push_back(to);
      }
    }
  }
  return dist;
}
