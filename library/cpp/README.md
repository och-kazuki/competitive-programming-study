# C++ ライブラリ

ヘッダは `library/cpp/` に置きます。コンパイル時のインクルードパスにはリポジトリのルートが入っているので、解答ファイルからは次のように読みます。

```cpp
#include "library/cpp/template.hpp"
```

`template.hpp` は入出力ヘルパーやマクロをまとめた共通ヘッダです。新しいアルゴリズムは同じディレクトリにヘッダを足し、解答側で include します。

```cpp
#include "library/cpp/template.hpp"
#include "library/cpp/your_header.hpp"
```

`#include <atcoder/...>` は AtCoder Library です。`nix develop` がパスを通します。問題ごとに使うときは `a.cpp` に書きます。

提出前に、引用符の include を1ファイルへ展開します。

```sh
just submit-file work/abc400/a
```

`work/abc400/a/submit.cpp` ができます。標準出力へ出すだけなら `just bundle work/abc400/a/a.cpp` です。`#include <atcoder/...>` は展開せず残します。AtCoder のジャッジには AtCoder Library があるためです。

テストは `tests/cpp/` に `*.cpp` を置きます。`main` から終了コード 0 で戻るプログラムにしてください。ファイルが無くても `just check` は通ります。追加したテストはコンパイルして実行されます。
