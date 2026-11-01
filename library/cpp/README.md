# C++ ライブラリ

ヘッダは `library/cpp/` に置きます。コンパイル時のインクルードパスにはリポジトリのルートが入っているので、テンプレートや作業ファイルからは次のように読みます。

```cpp
#include "library/cpp/binary_search.hpp"
```

`#include <atcoder/...>` は AtCoder Library です。`nix develop` がパスを通します。

提出前に、引用符の include を1ファイルへ展開します。

```sh
just bundle path/to/a.cpp > submit.cpp
```

PyPI の online-judge-tools 11 系には `oj-bundle` コマンドがありません。代わりに `scripts/bundle_cpp.py` が `#include "..."` だけを展開します。`#include <atcoder/...>` は残します。AtCoder のジャッジには AtCoder Library があるためです。

テストは `tests/cpp/` に置き、`just check` でコンパイルして実行します。新しいヘッダを足したら、同じ場所にテストを追加してください。
