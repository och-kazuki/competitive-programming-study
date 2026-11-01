#include <atcoder/dsu>

int main() {
  atcoder::dsu d(3);
  d.merge(0, 1);
  if (!d.same(0, 1)) {
    return 1;
  }
  if (d.same(0, 2)) {
    return 1;
  }
  if (d.size(0) != 2) {
    return 1;
  }
  return 0;
}
