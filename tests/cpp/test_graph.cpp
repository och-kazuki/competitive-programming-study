#include "library/cpp/graph/bfs01/bfs01.hpp"
#include "library/cpp/graph/dijkstra/dijkstra.hpp"

#include <cstdio>
#include <random>

static int failures = 0;

static void expect(bool ok, const char *msg) {
  if (!ok) {
    std::fprintf(stderr, "fail: %s\n", msg);
    ++failures;
  }
}

static vi floyd(const vc<vc<pi>> &g) {
  int n = (int)g.size();
  vvc<ll> d(n, vi(n, INF));
  for (int i = 0; i < n; i++) {
    d[i][i] = 0;
  }
  for (int v = 0; v < n; v++) {
    for (auto [to, cost] : g[v]) {
      d[v][to] = min(d[v][to], cost);
    }
  }
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      if (d[i][k] == INF) {
        continue;
      }
      for (int j = 0; j < n; j++) {
        if (d[k][j] == INF) {
          continue;
        }
        d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
      }
    }
  }
  vi flat;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      flat.push_back(d[i][j]);
    }
  }
  return flat;
}

static void test_dijkstra() {
  std::mt19937 rng(3);
  for (int trial = 0; trial < 40; trial++) {
    int n = 1 + (int)(rng() % 8);
    vc<vc<pi>> g(n);
    int m = (int)(rng() % 16);
    for (int i = 0; i < m; i++) {
      int a = (int)(rng() % n);
      int b = (int)(rng() % n);
      ll c = (ll)(rng() % 10);
      g[a].push_back({b, c});
    }
    auto all = floyd(g);
    for (int s = 0; s < n; s++) {
      auto res = dijkstra(g, s);
      for (int t = 0; t < n; t++) {
        expect(res.dist[t] == all[s * n + t], "ダイクストラ");
      }
      for (int t = 0; t < n; t++) {
        if (t == s || res.dist[t] == INF) {
          continue;
        }
        ll cost = 0;
        int v = t;
        int guard = 0;
        while (v != s) {
          int p = res.parent[v];
          expect(p != -1, "親");
          ll need = res.dist[v] - res.dist[p];
          bool found = false;
          for (auto [to, w] : g[p]) {
            if (to == v && w == need) {
              found = true;
              break;
            }
          }
          expect(found, "辺");
          cost += need;
          v = p;
          expect(++guard <= n, "単純路");
        }
        expect(cost == res.dist[t], "パスの長さ");
      }
    }
  }
}

static void test_bfs01() {
  std::mt19937 rng(4);
  for (int trial = 0; trial < 40; trial++) {
    int n = 1 + (int)(rng() % 10);
    vc<vc<pi>> g(n);
    int m = (int)(rng() % 20);
    for (int i = 0; i < m; i++) {
      int a = (int)(rng() % n);
      int b = (int)(rng() % n);
      g[a].push_back({b, (ll)(rng() % 2)});
    }
    for (int s = 0; s < n; s++) {
      expect(bfs01(g, s) == dijkstra(g, s).dist, "01-BFS");
    }
  }
}

int main() {
  test_dijkstra();
  test_bfs01();
  if (failures) {
    std::fprintf(stderr, "%d failures\n", failures);
    return 1;
  }
  return 0;
}
