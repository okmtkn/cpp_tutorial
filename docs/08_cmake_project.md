# 第8章 CMake実践 — 複数ファイル分割と静的ライブラリ

ここまで main.cpp 1つで書いてきましたが、実際のプロジェクトはファイルを分割します。
この章では **ヘッダ/ソース分割 → 静的ライブラリ化 → フォルダ構成** という
実務スタイルのプロジェクトを CMake で組み立てます。

サンプルコード: [`examples/ch08_cmake_lib/`](../examples/ch08_cmake_lib/)

## 8.1 なぜファイルを分けるのか

- 1ファイル1000行は読めない・探せない・直せない
- 数値積分などの汎用機能は、**次のプロジェクトでも再利用したい**
- 分割していれば、変更したファイルだけ再コンパイルされる（ビルドが速い）

## 8.2 ヘッダとソースの分割（復習＋C++流）

Cでやったのと同じ考え方です。**宣言をヘッダ(.hpp)に、定義をソース(.cpp)に**書きます。

`integrate.hpp` — 宣言（「こういう関数があります」という目次）:

```cpp
#pragma once   // 二重インクルード防止（#ifndef ガードの現代的な書き方）

#include <functional>

namespace numlib {

// 台形則による数値積分: f を [a, b] で n 分割して積分する
double trapezoid(const std::function<double(double)>& f,
                 double a, double b, int n = 1000);

}  // namespace numlib
```

`integrate.cpp` — 定義（中身の実装）:

```cpp
#include "numlib/integrate.hpp"

namespace numlib {

double trapezoid(const std::function<double(double)>& f,
                 double a, double b, int n) {
    const double h = (b - a) / n;
    double s = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i) {
        s += f(a + i * h);
    }
    return s * h;
}

}  // namespace numlib
```

新しい要素:

- **`#pragma once`** … `#ifndef XXX_H` / `#define` / `#endif` のガードと同じ役割の1行版
- **`namespace numlib { }`** … 自作ライブラリの関数を `numlib::trapezoid` のように
  名前空間で包む。他のライブラリと名前が衝突しなくなる（`std::` と同じ仕組み）
- **`std::function<double(double)>`** … 「double を受けて double を返す関数」を
  受け取れる型。関数もラムダ式も渡せる

## 8.3 実務スタイルのフォルダ構成

```
ch08_cmake_lib/
├── CMakeLists.txt
├── include/
│   └── numlib/              ← ヘッダは include/ライブラリ名/ に置く
│       ├── integrate.hpp
│       └── statistics.hpp
├── src/
│   ├── integrate.cpp        ← ライブラリの実装
│   └── statistics.cpp
└── apps/
    └── main.cpp             ← ライブラリを使うアプリ本体
```

ヘッダを `include/numlib/` に置くのは、使う側が

```cpp
#include "numlib/integrate.hpp"
```

と**どのライブラリのヘッダか分かる形**で書けるようにするためです。

## 8.4 CMakeLists.txt — ライブラリを定義する

```cmake
cmake_minimum_required(VERSION 3.21)
project(ch08_cmake_lib CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# (1) 静的ライブラリ numlib を作る
add_library(numlib STATIC
    src/integrate.cpp
    src/statistics.cpp
)

# (2) numlib のヘッダの場所を教える
#     PUBLIC = numlib 自身にも、numlib を使う側にも適用される
target_include_directories(numlib PUBLIC include)

# (3) 実行ファイルを作り、numlib をリンクする
add_executable(ch08_app apps/main.cpp)
target_link_libraries(ch08_app PRIVATE numlib)
```

新しいコマンドは3つだけです。

| コマンド | 意味 |
|----------|------|
| `add_library(名前 STATIC ソース...)` | ソース群を**静的ライブラリ**（.a ファイル）にまとめる |
| `target_include_directories(対象 PUBLIC dir)` | `#include` の検索パスを追加する |
| `target_link_libraries(対象 PRIVATE lib)` | ライブラリを実行ファイルに**リンク**する |

> **PUBLIC と PRIVATE**
> - `PUBLIC` … その設定を「自分」と「自分を使う側」の両方に伝える
> - `PRIVATE` … 自分だけに適用（使う側には伝えない）
>
> numlib のヘッダパスは、numlib を使う ch08_app 側でも `#include "numlib/..."` を
> 解決するために必要なので `PUBLIC` にしています。

## 8.5 使う側のコード

```cpp
// apps/main.cpp
#include <cmath>
#include <iostream>

#include "numlib/integrate.hpp"
#include "numlib/statistics.hpp"

int main() {
    // sin(x) を [0, π] で積分（理論値 2.0）
    constexpr double PI = 3.14159265358979323846;
    double s = numlib::trapezoid([](double x) { return std::sin(x); }, 0.0, PI);
    std::cout << "integral = " << s << "\n";

    std::vector<double> data = {2.1, 2.4, 1.9, 2.2, 2.0};
    auto [mean, sd] = numlib::mean_stddev(data);
    std::cout << "mean = " << mean << ", sd = " << sd << "\n";
    return 0;
}
```

ビルド・実行方法はこれまでと**まったく同じ**です（`F7` → ターゲット `ch08_app` を実行）。
ファイルが何十個に増えても手順が変わらないのが CMake の価値です。

## 8.6 add_subdirectory — プロジェクトの入れ子

このチュートリアルのルート `CMakeLists.txt` を開いてみてください。

```cmake
add_subdirectory(examples/ch01_hello)
add_subdirectory(examples/ch02_from_c)
...
```

`add_subdirectory(dir)` は「そのフォルダの CMakeLists.txt も読み込む」命令です。
大きなプロジェクトは、この仕組みで「ライブラリごとのフォルダ + それを束ねる親」という
構成を作ります。1〜8章のサンプルが一度にビルドできていたのはこのおかげです。

また、ルートで `set(CMAKE_CXX_STANDARD 17)` や `add_compile_options(-Wall -Wextra)` を
書いておくと、子フォルダ全体に効きます。**警告オプション `-Wall -Wextra` は常に付ける**
習慣にしてください。バグの多くは警告の段階で見つかります。

## 8.7 コンパイル時のマクロ定義（第5章の種明かし）

第5章で CSV ファイルのパスを埋め込んだ仕組みです。

```cmake
target_compile_definitions(ch05_csv_stats
    PRIVATE DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
```

これは gcc の `-DDATA_DIR="..."` オプションに相当し、C++ 側では

```cpp
std::string path = std::string(DATA_DIR) + "/sensor.csv";
```

のように「CMake が知っている情報（ソースのパス）」をコードに渡せます。
`${CMAKE_CURRENT_SOURCE_DIR}` は「この CMakeLists.txt があるフォルダ」を表す
CMake の変数です。

## 8.8 演習

1. サンプルの numlib に、第3章で作った統計関数を移植した
   `statistics.hpp / statistics.cpp` があります。ここに
   「最大値・最小値を返す `min_max`」を追加し、apps/main.cpp から呼んでください。
2. `apps/` に2つ目の実行ファイル `integral_table.cpp` を追加してください。
   内容: x², sin(x), e^x の [0,1] 積分値の表を表示する。
   CMakeLists.txt に何を足せばよいか考えましょう。
3. （発展）自分の `work/` フォルダに「ライブラリ + アプリ」構成のプロジェクトを
   ゼロから作ってみましょう。題材は自由です（単位換算ライブラリ、行列計算など）。

解答例: [`examples/ch08_cmake_lib/`](../examples/ch08_cmake_lib/)（演習1,2は組み込み済み）

次章: [第9章 外部ライブラリとテスト](09_external_lib_test.md)
