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

// 台形則による数値積分: 被積分関数 integrand を
// [lower_limit, upper_limit] で num_intervals 分割して積分する
double IntegrateTrapezoid(const std::function<double(double)>& integrand,
                          double lower_limit, double upper_limit,
                          int num_intervals = 1000);

}  // namespace numlib
```

`integrate.cpp` — 定義（中身の実装）:

```cpp
#include "numlib/integrate.hpp"

namespace numlib {

double IntegrateTrapezoid(const std::function<double(double)>& integrand,
                          double lower_limit, double upper_limit,
                          int num_intervals) {
    const double width = (upper_limit - lower_limit) / num_intervals;  // 台形1つの幅
    double sum = 0.5 * (integrand(lower_limit) + integrand(upper_limit));
    for (int i = 1; i < num_intervals; ++i) {
        sum += integrand(lower_limit + i * width);
    }
    return sum * width;
}

}  // namespace numlib
```

ヘッダは「使う人が最初に読む説明書」です。引数名を `f, a, b, n` ではなく
`integrand, lower_limit, upper_limit, num_intervals` にしておけば、
実装を読まなくても使い方が分かります。

新しい要素:

- **`#pragma once`** … `#ifndef XXX_H` / `#define` / `#endif` のガードと同じ役割の1行版
- **`namespace numlib { }`** … 自作ライブラリの関数を `numlib::IntegrateTrapezoid` のように
  名前空間で包む。他のライブラリと名前が衝突しなくなる（`std::` と同じ仕組み）
- **`std::function<double(double)>`** … 「double を受けて double を返す関数」を
  受け取れる型。関数もラムダ式も渡せる
- **デフォルト引数は宣言（ヘッダ）側にだけ書く** … `num_intervals = 1000` はヘッダにだけあり、
  `.cpp` の定義には書いていません。使う側が見るのはヘッダなので既定値はそこに書き、
  定義側にも `= 1000` を書くと「デフォルト引数の再定義」というコンパイルエラーになります

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

# 警告を多めに出す（初学者ほど警告に助けられます）
add_compile_options(-Wall -Wextra)

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
#include <vector>

#include "numlib/integrate.hpp"
#include "numlib/statistics.hpp"

int main() {
    // sin(x) を [0, π] で積分（理論値 2.0）
    constexpr double kPi = 3.14159265358979323846;
    const double sin_integral = numlib::IntegrateTrapezoid(
        [](double x) { return std::sin(x); }, 0.0, kPi);
    std::cout << "integral = " << sin_integral << "\n";

    std::vector<double> pin_diameters = {2.1, 2.4, 1.9, 2.2, 2.0};  // ピンの直径 [mm]
    auto [mean_diameter, stddev_diameter] = numlib::MeanAndStddev(pin_diameters);
    std::cout << "mean = " << mean_diameter << ", stddev = " << stddev_diameter << "\n";
    return 0;
}
```

ラムダの引数 `x` は数式 f(x) の x そのものなので、短い名前のままの方がかえって読みやすい例です。

ビルド・実行方法はこれまでと**まったく同じ**です（`F7` → ターゲット `ch08_app` を実行）。
ファイルが何十個に増えても手順が変わらないのが CMake の価値です。

## 8.6 add_subdirectory — プロジェクトの入れ子

このチュートリアルのサンプルは1つの CMakeLists.txt で完結していますが、
プロジェクトが大きくなると、ライブラリごとにフォルダを分けて
それぞれに CMakeLists.txt を置くことがよくあります。

```
my_project/
├── CMakeLists.txt       ← 親: 全体をまとめる
├── numlib/
│   ├── CMakeLists.txt   ← 子: add_library(numlib ...) を書く
│   ├── include/
│   └── src/
└── apps/
    ├── CMakeLists.txt   ← 子: add_executable(app ...) を書く
    └── main.cpp
```

```cmake
# my_project/CMakeLists.txt（親）
cmake_minimum_required(VERSION 3.21)
project(my_project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

add_subdirectory(numlib)
add_subdirectory(apps)
```

`add_subdirectory(dir)` は「そのフォルダの CMakeLists.txt も読み込む」命令です。
こうすると「ライブラリごとのフォルダ + それを束ねる親」という構成を作れます。
ターゲット名はプロジェクト全体で共通なので、`apps/CMakeLists.txt` では
`numlib/` 側で作った `numlib` をそのまま `target_link_libraries` に書けます。

また、親で `set(CMAKE_CXX_STANDARD 17)` や `add_compile_options(-Wall -Wextra)` を
書いておくと、子フォルダ全体に効きます。

## 8.7 コンパイル時のマクロ定義（第5章の種明かし）

第5章で CSV ファイルのパスを埋め込んだ仕組みです。

```cmake
target_compile_definitions(ch05_csv_stats
    PRIVATE DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
```

これは gcc の `-DDATA_DIR="..."` オプションに相当し、C++ 側では

```cpp
std::string csv_path = std::string(DATA_DIR) + "/sensor.csv";
```

のように「CMake が知っている情報（ソースのパス）」をコードに渡せます。
`${CMAKE_CURRENT_SOURCE_DIR}` は「この CMakeLists.txt があるフォルダ」を表す
CMake の変数です。

## 8.8 演習

サンプルには演習1・2の解答が組み込み済みなので、`examples/ch08_cmake_lib` から
`MinMax` と `integral_table.cpp` を除いた形で `work\ch08` にコピーし、そこで取り組んでください。

1. サンプルの numlib に、第3章で作った統計関数を移植した
   `statistics.hpp / statistics.cpp` があります。ここに
   「最小値・最大値を返す `MinMax`」を追加し、apps/main.cpp から呼んでください。
2. `apps/` に2つ目の実行ファイル `integral_table.cpp` を追加してください。
   内容: x², sin(x), e^x の [0,1] 積分値の表を表示する。
   CMakeLists.txt に何を足せばよいか考えましょう。
3. （発展）自分の `work/` フォルダに「ライブラリ + アプリ」構成のプロジェクトを
   ゼロから作ってみましょう。題材は自由です（単位換算ライブラリ、行列計算など）。

解答例: [`examples/ch08_cmake_lib/`](../examples/ch08_cmake_lib/)（演習1,2は組み込み済み）

次章: [第9章 外部ライブラリとテスト](09_external_lib_test.md)
