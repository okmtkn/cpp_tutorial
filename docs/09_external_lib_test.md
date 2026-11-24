# 第9章 外部ライブラリ (FetchContent) とテスト (CTest)

世の中には高品質なC++ライブラリが無数にあります（行列計算の Eigen、
JSON の nlohmann/json、フォーマットの fmt など）。
この章では CMake の **FetchContent** で外部ライブラリを自動ダウンロードして使う方法と、
**CTest** による自動テストを学びます。

> **この章は独立プロジェクトです。**
> `examples/ch09_external` フォルダを **VS Code で単独で開いて**進めてください
> （ファイル → フォルダーを開く）。初回 configure に**ネット接続**と、
> 第0章で入れた **Git**（`git --version` が動くこと）が必要です。

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
#include <string>
#include <utility>
#include <vector>

int main() {
    std::string selected_material = "aluminum";
    double selected_modulus = 70.3;   // ヤング率 [GPa]

    // {} に順番に埋め込まれる。型指定は不要
    fmt::print("material: {}, E = {} GPa\n", selected_material, selected_modulus);

    // 書式指定の例: {:>8.1f} → 右寄せ・幅8文字・小数点以下1桁
    std::vector<std::pair<std::string, double>> young_modulus_table = {
        {"steel", 206.0}, {"aluminum", 70.3}, {"titanium", 106.0},
    };
    fmt::print("{:-^30}\n", " Young's modulus ");   // 中央寄せ・- 埋め
    for (const auto& [material, young_modulus] : young_modulus_table) {
        fmt::print("{:<12} {:>8.1f} GPa\n", material, young_modulus);  // 左寄せ / 右寄せ
    }
    return 0;
}
```

出力:

```
material: aluminum, E = 70.3 GPa
------ Young's modulus -------
steel           206.0 GPa
aluminum         70.3 GPa
titanium        106.0 GPa
```

書式指定 `{:...}` の読み方:

| 書式 | 意味 |
|------|------|
| `{:<12}` | 左寄せ（`<`）で幅12文字 |
| `{:>8.1f}` | 右寄せ（`>`）で幅8文字、小数点以下1桁の固定小数点（`f`） |
| `{:-^30}` | 中央寄せ（`^`）で幅30文字、余白を `-` で埋める（余白が奇数なら右側が1つ多くなる） |

（サンプルの `src/main.cpp` は、これに加えて `mathutil` の関数を使った出力も行います）

`std::cout` の桁揃え（`std::setw` など）よりはるかに書きやすく、
実験結果の表出力・レポート作成に即戦力です。

## 9.3 自動テスト — なぜ書くのか

数値計算のコードは「なんとなく動いてるが実は間違っている」が起こりがちです。

たとえば、台形公式の積分関数を「もっと速くしよう」と書き換えたとします。
ループの範囲を1つ間違えて、端の区間を1つ足し忘れてしまいました。
それでも結果は 2.000 のはずが 1.998 になる程度で、画面に並んだ数字をざっと眺めても気づけません。
その関数を使った振動シミュレーションの結果が少しずつずれ、原因探しに何日もかかる…
というのは、研究でも実務でもよくある話です。

**理論値が分かっているケースをテストとして書いておけば**、
コードを書き換えるたびに全ケースを機械的に検証でき、こうした間違いにすぐ気づけます。
実験装置を校正するときに、値が分かっている**基準試料**を測って確かめるのと同じ考え方です。

テストの考え方:

1. 答えが分かっている入力を用意する（例: sin を 0→π で積分すると 2）
2. 関数を呼んで結果と比較する
3. 一致しなければテスト失敗として報告する

## 9.4 CTest でテストを実行する

CMake にはテストランナー **CTest** が付属しています。
「テスト用の小さな実行ファイル」を作って登録するだけです。

テストする対象は、サンプルの `src/mathutil.hpp` / `src/mathutil.cpp` にある小さな自作ライブラリ
`mathutil` です。中身は第3章で作った台形則の積分 `IntegrateSinTrapezoid`、平均 `Mean`、
空のデータなら `std::nullopt` を返す平均 `SafeMean`（第7章の optional）だけです。
CMakeLists.txt では第8章と同じく `add_library(mathutil STATIC src/mathutil.cpp)` でライブラリにしています。

```cmake
# ---- テスト ----
enable_testing()   # CTest を有効化

add_executable(test_mathutil tests/test_mathutil.cpp)
target_link_libraries(test_mathutil PRIVATE mathutil)

# 「test_mathutil を実行して、終了コード 0 なら合格」というテストを登録
add_test(NAME mathutil COMMAND test_mathutil)
```

テスト本体は `assert` ベースで簡潔に書けます:

```cpp
// tests/test_mathutil.cpp
#include <cassert>
#include <cmath>
#include "mathutil.hpp"

constexpr double kPi = 3.14159265358979323846;

// 浮動小数点の比較は「ぴったり一致」ではなく「誤差以内」で
bool IsNear(double actual, double expected, double tolerance = 1e-6) {
    return std::abs(actual - expected) < tolerance;
}

int main() {
    // 台形則: sin の [0, π] 積分は 2
    assert(IsNear(IntegrateSinTrapezoid(0.0, kPi, 10000), 2.0, 1e-4));

    // 平均: {1,2,3} の平均は 2
    assert(IsNear(Mean({1.0, 2.0, 3.0}), 2.0));

    // 空データの平均は optional の nullopt になるべき
    assert(!SafeMean({}).has_value());

    return 0;   // ここまで到達 = 全部合格
}
```

`assert(条件)` は、条件が偽ならメッセージを表示してプログラムを**異常終了**させます（Cの `assert` と同じ）。
異常終了すると終了コードが 0 以外になるので、CTest はそれを「不合格」と判定します。

> **注意: Release ビルドではテストが素通りする**
> `assert` は、リリース用のビルド（`NDEBUG` というマクロが定義される）では**丸ごと消えます**。
> そうなると、どんなバグがあっても「合格」になってしまいます。
> このチュートリアルでは、CMake Tools のビルドバリアントは既定の **Debug** のまま使ってください
> （ターミナルで `cmake -S . -B build -G Ninja` とした場合も `NDEBUG` は付かないので大丈夫です）。

`bool` を返す関数は `IsNear` のように `Is〜` / `Has〜` で始めると、
`if (IsNear(...))` や `assert(IsNear(...))` が「〜に近いなら」と自然に読めます。
引数も `a, b` ではなく `actual`（実際の値）と `expected`（期待値）にしておくと、
どちらに計算結果を渡すのか迷いません。

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
コマンドパレットの「**CMake: テストの実行**」（CMake: Run Tests）や、
左端のフラスコ型アイコン（テスト）のパネルからも実行できます
（CMake Tools のバージョンによって、ボタンの場所は多少異なります）。

> **本格的なテストフレームワーク**
> 実務では GoogleTest や Catch2、doctest といったテスト専用ライブラリを
> FetchContent で導入するのが定番です。仕組みは今回と同じ
> 「テスト実行ファイルを作って add_test で登録」なので、この章の知識で移行できます。

## 9.5 演習

1. サンプルの `mathutil` に「二乗平均平方根を返す `RootMeanSquare`」を追加し、
   `{3, 4}` の RMS が √12.5 ≒ 3.5355 になることのテストを書いてください。
2. **わざとバグを入れて**（例: 平均計算の割る数を `size()+1` にする）、
   ctest がちゃんと失敗を検出することを確認してください。
   テストの価値は「壊れたら気づける」ことです。
3. （発展）第5章のCSV集計の出力を fmt で桁の揃った表にしてみましょう。

解答例: [`examples/ch09_external/`](../examples/ch09_external/)（演習1は組み込み済み）

次章: [第10章 総合演習](10_final_project.md)
