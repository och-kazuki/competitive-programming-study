"""シードから1ケースを標準出力へ書く。"""

import random
import sys


def main() -> None:
    if len(sys.argv) != 2:
        print("使い方: gen.py <seed>", file=sys.stderr)
        sys.exit(1)
    rng = random.Random(int(sys.argv[1]))
    count = rng.randint(1, 5)
    values = [rng.randint(0, 9) for _ in range(count)]
    print(count)
    print(*values)


if __name__ == "__main__":
    main()
