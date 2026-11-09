// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/shortest_path
#define PROBLEM "https://judge.yosupo.jp/problem/shortest_path"

#include "library/cpp/graph/dijkstra/dijkstra.hpp"

int main() {
  int n, m, s, t;
  cin >> n >> m >> s >> t;
  vc<vc<pi>> g(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    ll c;
    cin >> a >> b >> c;
    g[a].push_back({b, c});
  }
  auto res = dijkstra(g, s);
  if (res.dist[t] == INF) {
    cout << -1 << '\n';
    return 0;
  }
  vc<pi> edges;
  for (int v = t; v != s; v = res.parent[v]) {
    edges.push_back({res.parent[v], v});
  }
  std::reverse(edges.begin(), edges.end());
  cout << res.dist[t] << ' ' << edges.size() << '\n';
  for (auto [u, v] : edges) {
    cout << u << ' ' << v << '\n';
  }
  return 0;
}
