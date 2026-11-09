#pragma once

#include "library/cpp/template.hpp"

// g[v] = (to, cost)。cost は 0 以上。届かない頂点の dist は INF。
struct Dijkstra {
  vi dist;
  vi parent;
};

inline Dijkstra dijkstra(const vc<vc<pi>> &g, int s) {
  int n = (int)g.size();
  Dijkstra res;
  res.dist.assign(n, INF);
  res.parent.assign(n, -1);
  pq_min<pi> pq;
  res.dist[s] = 0;
  pq.push({0, s});
  while (!pq.empty()) {
    auto [d, v] = pq.top();
    pq.pop();
    if (d != res.dist[v]) {
      continue;
    }
    for (auto [to, cost] : g[v]) {
      ll nd = d + cost;
      if (nd < res.dist[to]) {
        res.dist[to] = nd;
        res.parent[to] = v;
        pq.push({nd, to});
      }
    }
  }
  return res;
}
