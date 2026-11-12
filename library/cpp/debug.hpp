#pragma once

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
