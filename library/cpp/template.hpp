#pragma once

#include <bits/stdc++.h>
using namespace std;

// #include <atcoder/all>
// using namespace atcoder;

// init
struct Init {
  Init() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << setprecision(16);
  }
} init;

// input / output
template <typename... Args> void in(Args &...args) { (cin >> ... >> args); }
template <typename T> void invec(vector<T> &v, int n) {
  v.resize(n);
  for (auto &x : v)
    cin >> x;
}
template <typename... Args> void out(const Args &...args) {
  bool first = true;
  ((cout << (first ? "" : " ") << args, first = false), ...);
  cout << '\n';
}

template <class T> void vec_out(vector<T> &p, int ty = 0) {
  if (ty == 2) {
    cout << '{';
    for (int i = 0; i < (int)p.size(); i++) {
      if (i) {
        cout << ",";
      }
      cout << '"' << p[i] << '"';
    }
    cout << "}\n";
  } else {
    if (ty == 1) {
      cout << p.size() << "\n";
    }
    for (int i = 0; i < (int)(p.size()); i++) {
      if (i)
        cout << " ";
      cout << p[i];
    }
    cout << "\n";
  }
}
#define IN(...) in(__VA_ARGS__)
#define OUT(...) out(__VA_ARGS__)
#define VECIN(v, n) invec(v, n)
#define VECOUT(v, ty) vec_out(v, ty)

// Yes / No
#define YES cout << "Yes" << "\n"
#define NO cout << "No" << "\n"
#define YESNO(a) cout << (a ? "Yes" : "No") << "\n"

const int inf = 1e9 + 7;
const long long INF = 1LL << 60;

typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<vector<int>> vvi;
typedef pair<int, int> pi;

#define rep(i, a, b) for (int i = (a); i < (b); i++)
#define rrep(i, a, b) for (int i = (b) - 1; i >= (a); i--)
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
#define len(a) (int)a.size()

// chmin, chmax
template <class T> inline bool chmin(T &a, T b) {
  if (a > b) {
    a = b;
    return true;
  }
  return false;
}
template <class T> inline bool chmax(T &a, T b) {
  if (a < b) {
    a = b;
    return true;
  }
  return false;
}

// vector
template <class T> T vec_min(vector<T> &a) {
  assert(!a.empty());
  T ans = a[0];
  for (auto &x : a)
    chmin(ans, x);
  return ans;
}
template <class T> T vec_max(vector<T> &a) {
  assert(!a.empty());
  T ans = a[0];
  for (auto &x : a)
    chmax(ans, x);
  return ans;
}
template <class T> T vec_sum(vector<T> &a) {
  T ans = T(0);
  for (auto &x : a)
    ans += x;
  return ans;
}

// Grid
int dx[4] = {1, -1, 0, 0}; // R, L, D, U
int dy[4] = {0, 0, 1, -1}; // R, L, D, U
// int dx[8] = {1, 1, 0, 0, -1, -1};
// int dy[8] = {1, 0, -1, 1, 0, -1};
bool out_grid(int y, int x, int h, int w) {
  return (!(0 <= y && y < h && 0 <= x && x < w));
}
