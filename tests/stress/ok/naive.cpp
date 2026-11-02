#include <iostream>

int main() {
  long long value = 0;
  if (!(std::cin >> value)) {
    return 1;
  }
  std::cout << value << "\n";
  return 0;
}
