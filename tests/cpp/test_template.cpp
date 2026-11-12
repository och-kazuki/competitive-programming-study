#include "library/cpp/template.hpp"

#include <cstdio>
#include <sstream>

static int failures = 0;

static void expect(bool ok, const char *msg) {
  if (!ok) {
    std::fprintf(stderr, "fail: %s\n", msg);
    ++failures;
  }
}

int init = 1;

static void test_types() {
  static_assert(inf == (1 << 30) - 1, "inf");
  static_assert(INF == (ll(1) << 60), "INF");
  static_assert(std::is_same<vi, vector<ll>>::value, "vi は vector<ll>");
  static_assert(std::is_same<u32, uint32_t>::value, "u32");
  static_assert(std::is_same<u64, uint64_t>::value, "u64");
  static_assert(std::is_same<pi, pair<ll, ll>>::value, "pi");
  static_assert(std::is_same<vc<int>, vector<int>>::value, "vc");
  static_assert(std::is_same<vvc<int>, vector<vector<int>>>::value, "vvc");
  pq_max<ll> hi;
  pq_min<ll> lo;
  hi.push(1);
  hi.push(3);
  lo.push(1);
  lo.push(3);
  expect(hi.top() == 3, "pq_max");
  expect(lo.top() == 1, "pq_min");
  expect(init == 1, "init name");
  init = 2;
  expect(init == 2, "assign init");
}

static void test_io() {
  stringstream in("3 4 5 hello A 1.5 1 2 3 9 8 1 2 3 4");
  stringstream out;
  auto *old_in = cin.rdbuf(in.rdbuf());
  auto *old_out = cout.rdbuf(out.rdbuf());

  INT(n, m);
  LL(a);
  STR(s);
  CHAR(c);
  DBL(x);
  VEC(ll, v, 3);
  pi p;
  read(p);
  VV(ll, g, 2, 2);

  expect(n == 3 && m == 4, "INT");
  expect(a == 5, "LL");
  expect(s == "hello", "STR");
  expect(c == 'A', "CHAR");
  expect(x == 1.5, "DBL");
  expect(v == vi({1, 2, 3}), "VEC");
  expect(p.first == 9 && p.second == 8, "pair read");
  expect(g.size() == 2 && g[0] == vi({1, 2}) && g[1] == vi({3, 4}), "VV");

  print(v);
  print(g);
  vvc<vi> cube{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};
  print(cube);
  print(p);
  print();
  YES(true);
  YES(false);
  Yes(true);
  Yes(false);
  print(1, 2, 3);

  cin.rdbuf(old_in);
  cout.rdbuf(old_out);

  const string got = out.str();
  const string want = "1 2 3\n1 2\n3 4\n1 2\n3 4\n5 6\n7 8\n9 8\n\nYES\nNO\nYes\nNo\n1 2 3\n";
  expect(got == want, "print");
  if (got != want) {
    std::fprintf(stderr, "got:\n%s\n", got.c_str());
  }
}

static void test_loops() {
  vi forward;
  FOR(i, 3) forward.eb(i);
  expect(forward == vi({0, 1, 2}), "FOR(i, n)");

  vi ranged;
  FOR(i, 2, 5) ranged.eb(i);
  expect(ranged == vi({2, 3, 4}), "FOR(i, l, r)");

  vi stepped;
  FOR(i, 0, 6, 2) stepped.eb(i);
  expect(stepped == vi({0, 2, 4}), "FOR step");

  vi backward;
  FOR_R(i, 3) backward.eb(i);
  expect(backward == vi({2, 1, 0}), "FOR_R(i, n)");

  vi back_range;
  FOR_R(i, 1, 4) back_range.eb(i);
  expect(back_range == vi({3, 2, 1}), "FOR_R(i, l, r)");

  ll times = 0;
  FOR(4) times += 1;
  FOR_R(4) times += 1;
  expect(times == 8, "count loops");

  ll nested = 0;
  FOR(i, 2) {
    FOR(3) {
      FOR(j, i, i + 2) nested += j;
    }
  }
  expect(nested == 12, "nested");

  ll zero = 0;
  FOR(0) zero += 1;
  FOR(i, 0) zero += 1;
  FOR(i, 5, 5) zero += 1;
  FOR(i, 1, 1, 1) zero += 1;
  FOR_R(0) zero += 1;
  FOR_R(i, 0) zero += 1;
  FOR_R(i, 3, 3) zero += 1;
  vi empty;
  FOR(i, empty.size()) zero += 1;
  FOR_R(i, empty.size()) zero += 1;
  FOR_R(empty.size()) zero += 1;
  expect(zero == 0, "zero iterations");
}

static void test_helpers() {
  ll a = 5;
  expect(chmin(a, 3) && a == 3, "chmin changes");
  expect(!chmin(a, 3) && a == 3, "chmin same");
  expect(!chmin(a, 9) && a == 3, "chmin larger");
  expect(chmax(a, 9) && a == 9, "chmax changes");
  expect(!chmax(a, 1) && a == 9, "chmax smaller");
  expect(chmin(a, 4) && a == 4, "chmin int");

  vi values{3, 1, 2};
  expect(SUM<ll>(values) == 6, "SUM");

  vi sorted{1, 2, 2, 4};
  expect(LB(sorted, 2) == 1, "LB");
  expect(UB(sorted, 2) == 3, "UB");
  expect(LB(sorted, 0) == 0 && UB(sorted, 9) == 4, "LB UB ends");

  vi dup{1, 3, 2, 3};
  dup.reserve(32);
  const auto cap = dup.capacity();
  UNIQUE(dup);
  expect(dup == vi({1, 2, 3}), "UNIQUE");
  expect(dup.capacity() == cap, "UNIQUE does not shrink");

  vi seq{2, 1};
  sort(all(seq));
  expect(seq == vi({1, 2}) && len(seq) == 2, "all len");
  seq.eb(5);
  expect(seq.back() == 5, "eb");
}

static int probes = 0;

[[maybe_unused]] static int probe() {
  ++probes;
  return 7;
}

static void test_show() {
  stringstream out;
  stringstream err;
  auto *old_out = cout.rdbuf(out.rdbuf());
  auto *old_err = cerr.rdbuf(err.rdbuf());
  print(42);
  int side = 0;
  SHOW(++side);
  SHOW(probe());
  vi row{1, 2};
  vvc<ll> grid{{1, 2}, {3}};
  pi point{4, 5};
  SHOW(row, grid, point);
  const string stdout_text = out.str();
  const string stderr_text = err.str();
  cout.rdbuf(old_out);
  cerr.rdbuf(old_err);

  expect(stdout_text == "42\n", "stdout stays plain");
  expect(stdout_text.find("side") == string::npos, "debug is not on stdout");
  expect(stdout_text.find('[') == string::npos, "debug brackets are not on stdout");

#ifdef LOCAL
  expect(side == 1, "SHOW evaluates");
  expect(probes == 1, "SHOW evaluates call");
  expect(stderr_text.find("++side") != string::npos, "SHOW name");
  expect(stderr_text.find("probe") != string::npos, "SHOW call name");
  expect(stderr_text.find("test_show") != string::npos, "SHOW func");
  expect(stderr_text.find(':') != string::npos, "SHOW line");
  expect(stderr_text.find("[1, 2]") != string::npos, "SHOW vector");
  expect(stderr_text.find("[[1, 2], [3]]") != string::npos, "SHOW 2d");
  expect(stderr_text.find("(4, 5)") != string::npos, "SHOW pair");
#else
  expect(side == 0, "SHOW does not evaluate");
  expect(probes == 0, "SHOW does not call");
  expect(stderr_text.empty(), "SHOW writes nothing");
#endif
}

int main() {
  test_types();
  test_io();
  test_loops();
  test_helpers();
  test_show();
  return failures == 0 ? 0 : 1;
}
