"""単調な述語に対する二分探索。"""

from collections.abc import Callable


def binary_search_min(ng: int, ok: int, pred: Callable[[int], bool]) -> int:
    """pred(x) が真になる最小の整数を返す。

    ng < ok であること。範囲内がすべて偽なら、呼び出し時の ok を返す。
    """
    if ng >= ok:
        raise ValueError("ng < ok である必要があります")
    while ok - ng > 1:
        mid = ng + (ok - ng) // 2
        if pred(mid):
            ok = mid
        else:
            ng = mid
    return ok
