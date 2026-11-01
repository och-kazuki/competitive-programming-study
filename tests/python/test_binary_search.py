import unittest

from binary_search import binary_search_min


class BinarySearchTest(unittest.TestCase):
    def test_square(self) -> None:
        got = binary_search_min(-1, 101, lambda x: x * x >= 100)
        self.assertEqual(got, 10)

    def test_lower_bound(self) -> None:
        got = binary_search_min(0, 10, lambda x: x >= 3)
        self.assertEqual(got, 3)

    def test_all_true(self) -> None:
        got = binary_search_min(4, 100, lambda _x: True)
        self.assertEqual(got, 5)

    def test_all_false(self) -> None:
        got = binary_search_min(0, 10, lambda _x: False)
        self.assertEqual(got, 10)

    def test_invalid_range(self) -> None:
        with self.assertRaises(ValueError):
            binary_search_min(5, 5, lambda _x: True)


if __name__ == "__main__":
    unittest.main()
