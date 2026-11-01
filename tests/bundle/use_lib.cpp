#include "library/cpp/binary_search.hpp"
#include "library/cpp/binary_search.hpp"

#include <iostream>

int main() {
  long long answer = binary_search_min(0, 100, [](long long x) { return x >= 7; });
  if (answer != 7) {
    return 1;
  }
  std::cout << answer << "\n";
  return 0;
}
