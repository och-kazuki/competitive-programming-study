#include "library/cpp/binary_search.hpp"

#include <iostream>

namespace {

int failures = 0;

void expect(bool cond, const char *message) {
  if (!cond) {
    std::cerr << "failed: " << message << "\n";
    ++failures;
  }
}

} // namespace

int main() {
  const long long square =
      binary_search_min(-1, 101, [](long long x) { return x * x >= 100; });
  expect(square == 10LL, "square");
  expect(binary_search_min(0, 10, [](long long x) { return x >= 3; }) == 3LL, "lower");
  expect(binary_search_min(4, 100, [](long long) { return true; }) == 5LL, "all true");
  expect(binary_search_min(0, 10, [](long long) { return false; }) == 10LL, "all false");
  return failures == 0 ? 0 : 1;
}
