// Repository: https://github.com/och-kazuki/competitive-programming-study
// このテンプレートには、AIによるコード生成を利用した部分が含まれます。
#pragma once

#include <bits/stdc++.h>
using namespace std;

// #include <atcoder/all>
// using namespace atcoder;

// vi は vector<ll>。vector<int> ではない。
using ll = long long;
using u32 = uint32_t;
using u64 = uint64_t;
using pi = pair<ll, ll>;
using vi = vector<ll>;
template <class T> using vc = vector<T>;
template <class T> using vvc = vector<vector<T>>;
template <class T> using pq_max = priority_queue<T>;
template <class T> using pq_min = priority_queue<T, vector<T>, greater<T>>;

constexpr int inf = 1000000007;
constexpr ll INF = ll(1) << 60;

struct Init {
  Init() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << setprecision(16);
  }
} init;

template <class T> void read_one(T &x) { cin >> x; }

template <class T, class U> void read_one(pair<T, U> &p) {
  read_one(p.first);
  read_one(p.second);
}

// 確保済みの要素を、前から順に読む。resize はしない。二次元もこの再帰で読む。
template <class T> void read_one(vector<T> &v) {
  for (auto &x : v) {
    read_one(x);
  }
}

template <class... Args> void read(Args &...args) { (read_one(args), ...); }

template <class T> void print_one(const T &x);
template <class T, class U> void print_one(const pair<T, U> &p);
template <class T> void print_one(const vector<T> &v);
template <class T> void print_one(const vector<vector<T>> &v);

template <class T> void print_one(const T &x) { cout << x; }

template <class T, class U> void print_one(const pair<T, U> &p) {
  print_one(p.first);
  cout << ' ';
  print_one(p.second);
}

template <class T> void print_one(const vector<T> &v) {
  for (ll i = 0; i < static_cast<ll>(v.size()); ++i) {
    if (i) {
      cout << ' ';
    }
    print_one(v[i]);
  }
}

template <class T> void print_one(const vector<vector<T>> &v) {
  for (ll i = 0; i < static_cast<ll>(v.size()); ++i) {
    if (i) {
      cout << '\n';
    }
    print_one(v[i]);
  }
}

inline void print() { cout << '\n'; }

template <class T, class... Args> void print(const T &first, const Args &...rest) {
  print_one(first);
  if constexpr (sizeof...(rest) == 0) {
    cout << '\n';
  } else {
    cout << ' ';
    print(rest...);
  }
}

inline void YES(bool cond) { print(cond ? "YES" : "NO"); }
inline void Yes(bool cond) { print(cond ? "Yes" : "No"); }

template <class T, class U> inline bool chmin(T &a, const U &b) {
  if (b < a) {
    a = static_cast<T>(b);
    return true;
  }
  return false;
}

template <class T, class U> inline bool chmax(T &a, const U &b) {
  if (a < b) {
    a = static_cast<T>(b);
    return true;
  }
  return false;
}

template <class T, class A> T SUM(const A &a) {
  T sum = 0;
  for (const auto &x : a) {
    sum += static_cast<T>(x);
  }
  return sum;
}

// LB / UB はソート済みの配列に対する位置（0 始まり）。
template <class A, class U> ll LB(const A &a, const U &x) {
  return static_cast<ll>(std::lower_bound(std::begin(a), std::end(a), x) - std::begin(a));
}

template <class A, class U> ll UB(const A &a, const U &x) {
  return static_cast<ll>(std::upper_bound(std::begin(a), std::end(a), x) - std::begin(a));
}

// 先頭は 0。s[i] は a[0] + ... + a[i - 1]。
template <class T, class A> vc<T> cumsum(const A &a) {
  vc<T> s(std::size(a) + 1);
  for (ll i = 0; i < static_cast<ll>(std::size(a)); ++i) {
    s[i + 1] = s[i] + static_cast<T>(a[i]);
  }
  return s;
}

// 添字は ll、範囲は半開区間。境界はループに入る前に一度だけ評価する。
// 境界式に副作用は持たせない。回数だけの FOR / FOR_R は、入れ子でも変数名が重ならない。
// FOR_R は境界を ll にしてから減らすので、size() が 0 でも符号なしのアンダーフローは起きない。
#define CP_CAT2(a, b) a##b
#define CP_CAT(a, b) CP_CAT2(a, b)
#define CP_OVERLOAD4(_1, _2, _3, _4, NAME, ...) NAME
#define CP_OVERLOAD3(_1, _2, _3, NAME, ...) NAME

#define CP_FOR2(i, n, uid)                                                                     \
  for (ll i = 0, CP_CAT(_cp_end_, uid) = static_cast<ll>(n); i < CP_CAT(_cp_end_, uid); ++i)
#define CP_FOR1(n, uid) CP_FOR2(CP_CAT(_cp_i_, uid), n, uid)
#define CP_FOR3(i, l, r, uid)                                                                  \
  for (ll i = static_cast<ll>(l), CP_CAT(_cp_end_, uid) = static_cast<ll>(r);                 \
       i < CP_CAT(_cp_end_, uid); ++i)
#define CP_FOR4(i, l, r, step, uid)                                                            \
  for (ll i = static_cast<ll>(l), CP_CAT(_cp_end_, uid) = static_cast<ll>(r),                 \
           CP_CAT(_cp_step_, uid) = static_cast<ll>(step);                                    \
       i < CP_CAT(_cp_end_, uid); i += CP_CAT(_cp_step_, uid))

#define FOR1(n) CP_FOR1(n, __COUNTER__)
#define FOR2(i, n) CP_FOR2(i, n, __COUNTER__)
#define FOR3(i, l, r) CP_FOR3(i, l, r, __COUNTER__)
#define FOR4(i, l, r, step) CP_FOR4(i, l, r, step, __COUNTER__)
#define FOR(...) CP_OVERLOAD4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)

#define CP_FOR_R3(i, l, r, uid)                                                                \
  for (ll CP_CAT(_cp_l_, uid) = static_cast<ll>(l), i = static_cast<ll>(r);                   \
       i-- > CP_CAT(_cp_l_, uid);)
#define FOR_R3(i, l, r) CP_FOR_R3(i, l, r, __COUNTER__)
#define FOR_R2(i, n) FOR_R3(i, 0, n)
#define FOR_R1_IMPL(n, uid) FOR_R3(CP_CAT(_cp_i_, uid), 0, n)
#define FOR_R1(n) FOR_R1_IMPL(n, __COUNTER__)
#define FOR_R(...) CP_OVERLOAD3(__VA_ARGS__, FOR_R3, FOR_R2, FOR_R1)(__VA_ARGS__)

#define all(x) std::begin(x), std::end(x)
#define rall(x) std::rbegin(x), std::rend(x)
#define len(x) static_cast<ll>(std::size(x))
#define eb emplace_back

// ソートして重複を消す。並び順は変わる。shrink_to_fit は呼ばない。
#define UNIQUE(A)                                                                              \
  do {                                                                                         \
    std::sort(std::begin(A), std::end(A));                                                     \
    (A).erase(std::unique(std::begin(A), std::end(A)), std::end(A));                           \
  } while (0)

// マクロは文が二つになる。if の直後に書くときは波括弧で囲む。
// 対話問題では print のあと、明示的に cout << flush; する。
#define INT(...)                                                                               \
  int __VA_ARGS__;                                                                             \
  read(__VA_ARGS__)
#define LL(...)                                                                                \
  ll __VA_ARGS__;                                                                              \
  read(__VA_ARGS__)
#define STR(...)                                                                               \
  string __VA_ARGS__;                                                                          \
  read(__VA_ARGS__)
#define CHAR(...)                                                                              \
  char __VA_ARGS__;                                                                            \
  read(__VA_ARGS__)
#define DBL(...)                                                                               \
  double __VA_ARGS__;                                                                          \
  read(__VA_ARGS__)
#define VEC(type, name, n)                                                                     \
  vc<type> name(n);                                                                            \
  read(name)
#define VV(type, name, h, w)                                                                   \
  vvc<type> name((h), vc<type>(w));                                                            \
  read(name)

#ifdef LOCAL

template <class T> void show_write(const T &x);
inline void show_write(bool x);
inline void show_write(char x);
inline void show_write(const char *x);
inline void show_write(const string &x);
template <class T, class U> void show_write(const pair<T, U> &p);
template <class T> void show_write(const vector<T> &v);

template <class T> void show_write(const T &x) { cerr << x; }
inline void show_write(bool x) { cerr << (x ? "true" : "false"); }
inline void show_write(char x) { cerr << '\'' << x << '\''; }
inline void show_write(const char *x) { cerr << '"' << x << '"'; }
inline void show_write(const string &x) { cerr << '"' << x << '"'; }

template <class T, class U> void show_write(const pair<T, U> &p) {
  cerr << '(';
  show_write(p.first);
  cerr << ", ";
  show_write(p.second);
  cerr << ')';
}

template <class T> void show_write(const vector<T> &v) {
  cerr << '[';
  for (ll i = 0; i < static_cast<ll>(v.size()); ++i) {
    if (i) {
      cerr << ", ";
    }
    show_write(v[i]);
  }
  cerr << ']';
}

inline string show_name(const char *begin, const char *end) {
  while (begin < end && *begin == ' ') {
    ++begin;
  }
  while (end > begin && *(end - 1) == ' ') {
    --end;
  }
  return string(begin, end);
}

inline const char *show_split(const char *names) {
  int depth = 0;
  for (const char *p = names; *p; ++p) {
    if (*p == '(' || *p == '[' || *p == '<') {
      ++depth;
    } else if (*p == ')' || *p == ']' || *p == '>') {
      --depth;
    } else if (*p == ',' && depth == 0) {
      return p;
    }
  }
  return names + strlen(names);
}

template <class T> void show_args(const char *names, const T &value) {
  const char *end = show_split(names);
  cerr << show_name(names, end) << " = ";
  show_write(value);
}

template <class T, class... Args>
void show_args(const char *names, const T &value, const Args &...rest) {
  const char *comma = show_split(names);
  cerr << show_name(names, comma) << " = ";
  show_write(value);
  cerr << ", ";
  if (*comma == ',') {
    ++comma;
  }
  show_args(comma, rest...);
}

#define SHOW(...)                                                                              \
  do {                                                                                         \
    cerr << '[' << __func__ << ':' << __LINE__ << "] ";                                       \
    show_args(#__VA_ARGS__, __VA_ARGS__);                                                      \
    cerr << '\n';                                                                              \
  } while (0)

#else

// 引数は展開しない。
#define SHOW(...) ((void)0)

#endif

// グリッドの dx / dy と out_grid は Zed のスニペット（snippets/zed/c++.json）です。
// 必要な問題の a.cpp にだけ展開します。
