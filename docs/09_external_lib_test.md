# 第9章 外部ライブラリ (FetchContent) とテスト (CTest)

世の中には高品質なC++ライブラリが無数にあります（行列計算の Eigen、
JSON の nlohmann/json、フォーマットの fmt など）。
この章では CMake の **FetchContent** で外部ライブラリを自動ダウンロードして使う方法と、
**CTest** による自動テストを学びます。

> **この章は独立プロジェクトです。**
> `examples/ch09_external` フォルダを **VS Code で単独で開いて**進めてください
> （ファイル → フォルダーを開く）。初回 configure に**ネット接続が必要**です。

サンプルコード: [`examples/ch09_external/`](../examples/ch09_external/)

## 9.1 FetchContent — ライブラリを自動で取ってくる

昔のC++は外部ライブラリの導入が大変でした（手動ダウンロード、パス設定、ビルド…）。
FetchContent を使うと、**CMakeLists.txt に書くだけ**で configure 時に
GitHub から自動ダウンロード・ビルドされます。

```cmake
cmake_minimum_required(VERSION 3.21)
project(ch09_external CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- fmt ライブラリを取得 ----
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        11.1.4          # バージョンを固定するのが重要！
)
FetchContent_MakeAvailable(fmt)

add_executable(report src/main.cpp)
target_link_libraries(report PRIVATE fmt::fmt)   # あとはリンクするだけ
```

- `GIT_TAG` でバージョンを**固定**します。「いつビルドしても同じ結果」になることは、
  研究の再現性の観点でも重要です。
- ダウンロードされたものは `build/_deps/` に入ります。手元のソースは汚れません。

## 9.2 fmt — printf の良いとこ取りの出力ライブラリ

fmt は「Pythonの f文字列のような書式指定」ができる大人気ライブラリで、
C++20 の `std::format` / C++23 の `std::print` の元になったライブラリです。

```cpp
#include <fmt/core.h>
#include <vector>

int main() {
    std::string name = "aluminum";
    double E = 70.3;

    // {} に順番に埋め込まれる。型指定は不要
    fmt::print("material: {}, E = {} GPa\n", name, E);

    // 書式指定: {:8.2f} → 幅8桁・小数点以下2桁
    std::vector<std::pair<std::string, double>> table = {
        {"steel", 206.0}, {"aluminum", 70.3}, {"titanium", 106.0},
    };
    fmt::print("{:-^30}\n", " Young's modulus ");   // 中央寄せ・- 埋め
    for (const auto& [mat, e] : table) {
        fmt::print("{:<12} {:>8.1f} GPa\n", mat, e);  // 左寄せ / 右寄せ
    }
    return 0;
}
```

出力:

```
material: aluminum, E = 70.3 GPa
------ Young's modulus ------
steel           206.0 GPa
aluminum         70.3 GPa
titanium        106.0 GPa
```

`std::cout` の桁揃え（`std::setw` など）よりはるかに書きやすく、
実験結果の表出力・レポート作成に即戦力です。

## 9.3 自動テスト — なぜ書くのか

数値計算のコードは「なんとなく動いてるが実は間違っている」が起こりがちです。
**理論値が分かっているケースをテストとして書いておけば**、
コードを書き換えるたびに全ケースを機械的に検証できます。

テストの考え方:

1. 答えが分かっている入力を用意する（例: sin を 0→π で積分すると 2）
2. 関数を呼んで結果と比較する
3. 一致しなければテスト失敗として報告する

## 9.4 CTest でテストを実行する

CMake にはテストランナー **CTest** が付属しています。
「テスト用の小さな実行ファイル」を作って登録するだけです。

```cmake
# ---- テスト ----
enable_testing()   # CTest を有効化

add_executable(test_mathutil tests/test_mathutil.cpp)
target_link_libraries(test_mathutil PRIVATE mathutil)

# 「test_mathutil を実行して、終了コード 0 なら合格」というテストを登録
add_test(NAME mathutil COMMAND test_mathutil)
```

テスト本体は `assert` ベースで簡潔に書けます（サンプルでは少しリッチな
CHECK マクロを用意しています）:

```cpp
// tests/test_mathutil.cpp
#include <cassert>
#include <cmath>
#include "mathutil.hpp"

// 浮動小数点の比較は「ぴったり一致」ではなく「誤差以内」で
bool near(double a, double b, double tol = 1e-6) {
    return std::abs(a - b) < tol;
}

int main() {
    // 台形則: sin の [0, π] 積分は 2
    assert(near(trapezoid_sin(0.0, 3.14159265358979, 10000), 2.0, 1e-4));

    // 平均: {1,2,3} の平均は 2
    assert(near(mean({1.0, 2.0, 3.0}), 2.0));

    // 空データの平均は optional の nullopt になるべき
    assert(!mean_safe({}).has_value());

    return 0;   // ここまで到達 = 全部合格
}
```

### テストの実行

ビルド後、ターミナルで:

```powershell
ctest --test-dir build --output-on-failure
```

```
    Start 1: mathutil
1/1 Test #1: mathutil .........................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1
```

VS Code なら、CMake Tools がテストを認識し、
コマンドパレットの「**CMake: テストの実行**」やステータスバーのビーカーアイコンからも
実行できます。

> **本格的なテストフレームワーク**
> 実務では GoogleTest や Catch2、doctest といったテスト専用ライブラリを
> FetchContent で導入するのが定番です。仕組みは今回と同じ
> 「テスト実行ファイルを作って add_test で登録」なので、この章の知識で移行できます。

## 9.5 演習

1. サンプルの `mathutil` に「二乗平均平方根 `rms`」を追加し、
   `{3, 4}` の rms が √12.5 ≒ 3.5355 になることのテストを書いてください。
2. **わざとバグを入れて**（例: 平均計算の割る数を `size()+1` にする）、
   ctest がちゃんと失敗を検出することを確認してください。
   テストの価値は「壊れたら気づける」ことです。
3. （発展）第5章のCSV集計の出力を fmt で桁の揃った表にしてみましょう。

解答例: [`examples/ch09_external/`](../examples/ch09_external/)（演習1は組み込み済み）

次章: [第10章 総合演習](10_final_project.md)
