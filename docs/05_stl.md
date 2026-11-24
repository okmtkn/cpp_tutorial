# 第5章 STLとラムダ式 — 車輪の再発明をやめる

STL（標準テンプレートライブラリ）には、ソート・検索・集計などの定番処理が
すべて入っています。「forループを自分で書く前に、標準ライブラリにないか探す」が
C++ 上級者の習慣です。この章の最後では、実験データのCSVファイル処理を行います。

サンプルコード: [`examples/ch05_stl/`](../examples/ch05_stl/)

## 5.1 algorithm の定番関数

```cpp
#include <algorithm>   // sort, min_element, max_element など
#include <iostream>
#include <numeric>     // accumulate
#include <vector>

int main() {
    std::vector<double> temperatures = {23.5, 21.2, 25.8, 22.1, 24.9};  // [degC]

    // 合計と平均
    double temperature_sum =
        std::accumulate(temperatures.begin(), temperatures.end(), 0.0);
    std::cout << "mean: " << temperature_sum / temperatures.size() << "\n";

    // 最大・最小（イテレータが返るので * で値を取り出す）
    double max_temperature = *std::max_element(temperatures.begin(), temperatures.end());
    double min_temperature = *std::min_element(temperatures.begin(), temperatures.end());
    std::cout << "max: " << max_temperature << ", min: " << min_temperature << "\n";

    // 昇順ソート
    std::sort(temperatures.begin(), temperatures.end());
    for (double temperature : temperatures) std::cout << temperature << " ";
    std::cout << "\n";
    return 0;
}
```

- `temperatures.begin(), temperatures.end()` は「コンテナの先頭から末尾まで」という範囲指定の
  お約束です。ほとんどの algorithm 関数がこの形で範囲を受け取ります。
- `std::accumulate` の第3引数 `0.0` は合計の初期値です。
  **`0`（int）と書くと小数が切り捨てられるバグ**になるので注意！（典型的なハマりどころ）

## 5.2 ラムダ式 — その場で作る小さな関数

ラムダ式は、記号が並んでいて最初は難しそうに見えます。でも正体は
**「名前のない関数」**にすぎません。いきなり文法を覚えるのではなく、
みなさんがすでに書ける**普通の関数から少しずつ書き換えて**たどり着きましょう。

### ステップ1: algorithm に「判定のしかた」を教えたい

5.1 の `accumulate` や `max_element` は、やることが最初から決まっていました。
でも実際のデータ処理では「120 MPa を超えるデータは何個？」のように、
**条件を自分で決めたい**場面がよくあります。

そのために `std::count_if` は、「要素を1つ受け取って `true` / `false` を返す関数」を
引数として受け取れるようになっています。まずは、これまでに習った普通の関数で書いてみます。

```cpp
// 応力が 120 MPa を超えていれば true を返す関数
bool IsOver120(double stress) {
    return stress > 120.0;
}

int main() {
    std::vector<double> stresses = {120.5, 98.2, 141.7, 110.3, 135.2};  // [MPa]

    // 関数名だけを渡す（IsOver120() のように () は付けない）
    auto count_by_function =
        std::count_if(stresses.begin(), stresses.end(), IsOver120);
}
```

`count_if` は内部で、要素を1つずつ `IsOver120(要素)` に渡して呼び出し、
`true` が返ってきた回数を数えています。C言語で `qsort` に比較関数を渡したことがあれば、
それと同じ「**関数を引数として渡す**」発想です。

これでも正しく動きますが、不満が2つあります。

- 1回しか使わない2行の判定のために、名前を考えて `main` の外に関数を書く必要がある
- `count_if` の行を読んでも条件の中身が見えず、関数の定義を探しに行く必要がある

### ステップ2: 関数をその場に書く ＝ ラムダ式

普通の関数とラムダ式を、上下に並べて比べてみます。

```
bool IsOver120 (double stress) { return stress > 120.0; }   // 普通の関数
             [](double stress) { return stress > 120.0; }   // ラムダ式
```

変わったのは先頭だけです。

| 普通の関数 | ラムダ式 | 理由 |
|------------|----------|------|
| 戻り値の型 `bool` | 書かない | `return` 文から自動で決まる |
| 関数名 `IsOver120` | `[]` | 名前は不要。`[]` は「ここからラムダ式」という目印 |
| 引数`(double stress)` | 同じ | 引数の書き方は普通の関数と同じ |
| 関数の本体`{ return ...; }` | 同じ | 本体の書き方も普通の関数と同じ |

これを、関数名を書いていた場所（`count_if` の第3引数）にそのまま書きます。

```cpp
auto count_over_120 =
    std::count_if(stresses.begin(), stresses.end(),
                  [](double stress) { return stress > 120.0; });
```

条件が呼び出しの場所に書いてあるので、「応力が 120 を超えるものを数える」と
この1文だけで読めるようになりました。

### ステップ3: ラムダ式は本当に「関数」なのか確かめる

ラムダ式は変数に入れておき、普通の関数と同じように `()` で呼び出せます。

```cpp
auto is_over_120 = [](double stress) { return stress > 120.0; };
std::cout << is_over_120(141.7) << "\n";   // 1 (true)
std::cout << is_over_120(98.2) << "\n";    // 0 (false)
```

ラムダ式の型にはコンパイラが内部で付けた名前しかなく、私たちには書けません。
そのため、受け取る変数は `auto` で宣言します。

### ステップ4: 引数が2つのラムダ式 — 降順ソート

`std::sort` は何も指定しないと昇順に並べます。並べ方を変えたいときは、
「**2つの要素を受け取り、1つ目を前に置くべきなら `true` を返す関数**」を渡します。

```cpp
std::sort(stresses.begin(), stresses.end(),
          [](double lhs, double rhs) { return lhs > rhs; });
```

「左（`lhs`）のほうが大きければ前に置く」なので、大きい順（降順）になります。

### ステップ5: 外の変数を使う — 捕獲（キャプチャ）

しきい値 120.0 を直接書くのではなく、変数にしたくなったとします。
素直に書くと、実は**コンパイルエラー**になります。

```cpp
double stress_limit = 130.0;   // 許容応力 [MPa]
auto count_over_limit = std::count_if(
    stresses.begin(), stresses.end(),
    [](double stress) { return stress > stress_limit; });   // エラー！
// error: 'stress_limit' is not captured
```

ラムダ式の本体からは、**外側の局所変数が既定では見えない**のです
（普通の関数から `main` の局所変数が見えないのと同じです）。
使いたい変数は `[]` の中に書いて、ラムダ式の中へ「持ち込む」と宣言します。
これを**捕獲（キャプチャ）**と呼びます。

```cpp
[stress_limit](double stress) { return stress > stress_limit; }   // OK
```

`[]` は「ここからラムダ式」の目印であると同時に、**持ち込む変数を書く場所**でもあったわけです。

### まとめたプログラム

ステップ1〜5をまとめたのがサンプルの `lambda.cpp` です。

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

// ステップ1: 普通の関数で判定基準を作る
// 応力が 120 MPa を超えていれば true を返す
bool IsOver120(double stress) {
    return stress > 120.0;
}

int main() {
    std::vector<double> stresses = {120.5, 98.2, 141.7, 110.3, 135.2};  // [MPa]

    // ステップ1: 普通の関数を count_if に渡す（関数名だけを書く。() は付けない）
    auto count_by_function =
        std::count_if(stresses.begin(), stresses.end(), IsOver120);
    std::cout << "over 120 MPa (function): " << count_by_function << "\n";

    // ステップ2: 同じ判定をラムダ式でその場に書く
    auto count_over_120 =
        std::count_if(stresses.begin(), stresses.end(),
                      [](double stress) { return stress > 120.0; });
    std::cout << "over 120 MPa (lambda):   " << count_over_120 << "\n";

    // ステップ3: ラムダ式は変数に入れて、普通の関数のように呼べる
    auto is_over_120 = [](double stress) { return stress > 120.0; };
    std::cout << "is_over_120(141.7): " << is_over_120(141.7) << "\n";  // 1 (true)
    std::cout << "is_over_120(98.2):  " << is_over_120(98.2) << "\n";   // 0 (false)

    // ステップ4: 引数2つのラムダ — 降順ソート
    std::sort(stresses.begin(), stresses.end(),
              [](double lhs, double rhs) { return lhs > rhs; });
    for (double stress : stresses) std::cout << stress << " ";
    std::cout << "\n";

    // ステップ5: 外の変数を捕獲して使う
    double stress_limit = 130.0;  // 許容応力 [MPa]
    auto count_over_limit = std::count_if(
        stresses.begin(), stresses.end(),
        [stress_limit](double stress) { return stress > stress_limit; });
    std::cout << "over " << stress_limit << " MPa: " << count_over_limit
              << "\n";
    return 0;
}
```

### 文法のまとめ

```
[捕獲リスト](引数) { 本体 }
```

- `[]` … 外の変数を使わない
- `[stress_limit]` … 変数 stress_limit をコピーして使う
- `[&]` … 外の変数をすべて参照で使う（短いラムダで便利だが多用注意）

「名前を付けるほどでもない使い捨ての関数」をその場に書ける機能、と理解してください。
関数そのものには名前がなくても、**引数には `stress` のような中身が分かる名前**を付けると、
`[](double stress) { return stress > 120.0; }` が「応力が 120 を超える」とそのまま読めます。

## 5.3 std::map — キーと値の対応表

`std::map<キーの型, 値の型>` は辞書（対応表）です。

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    // 材料名 → ヤング率 [GPa]
    std::map<std::string, double> young_modulus_by_material{
        {"steel", 206.0},
        {"aluminum", 70.3},
        {"copper", 120.0},
    };

    young_modulus_by_material["titanium"] = 106.0;   // 追加も代入も []

    // 読むだけなら at()（キーが無ければ例外。勝手に登録されない）
    std::cout << "steel: " << young_modulus_by_material.at("steel") << " GPa\n";

    // 全要素を回す（キーの昇順に並ぶ）。構造化束縛が活躍
    for (const auto& [material, young_modulus] : young_modulus_by_material) {
        std::cout << material << ": " << young_modulus << " GPa\n";
    }

    // 存在チェックは count() か find() で
    if (young_modulus_by_material.count("concrete") == 0) {
        std::cout << "concrete is not registered\n";
    }
    return 0;
}
```

map の名前は `young_modulus_by_material`（材料ごとのヤング率）のように
「**何をキーに何を引く表か**」が分かる形にすると、`young_modulus_by_material["steel"]` が
「鋼のヤング率」とそのまま読めます。

> **注意:** `map[キー]` は**キーが存在しないと値0で勝手に登録**します。
> 「読むだけ」のつもりのときは `at()` か `find()` を使ってください
> （上のコードで steel を `at()` で読んでいるのはこのためです）。

## 5.4 ファイル入出力 — fstream

Cの `fopen`/`fclose` に相当するのが `std::ifstream`（読み）/`std::ofstream`（書き）です。
**close は自動で呼ばれる**のがポイントです（この仕組み＝RAIIは第6章で解説）。

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main() {
    // 書き込み: 1〜5 とその2乗の表
    {
        std::ofstream output_file("result.txt");
        output_file << "x, x^2\n";
        for (int value = 1; value <= 5; ++value) {
            output_file << value << ", " << value * value << "\n";
        }
    }   // output_file.close() は書かなくてよい（スコープを抜けると自動で閉じる）

    // 読み込み（1行ずつ）
    std::ifstream input_file("result.txt");
    if (!input_file) {   // 開けたかどうかのチェックは必須！
        std::cerr << "cannot open result.txt\n";
        return 1;
    }
    std::string line;
    while (std::getline(input_file, line)) {
        std::cout << line << "\n";
    }
    return 0;
}
```

> **`result.txt` はどこにできる？ — 作業ディレクトリ**
> `"result.txt"` のようにフォルダを書かないファイル名は、**プログラムを実行したときの
> 作業ディレクトリ（カレントディレクトリ）** を基準に探されます。
>
> - ターミナルで `.\build\ch05_fstream.exe` と実行した場合 → そのターミナルのいるフォルダ（章フォルダ直下）
> - VS Code の ▶（CMake Tools）で実行した場合 → 通常は exe のある `build` フォルダ
>
> 「ファイルが見当たらない」「読み込みに失敗する」ときは、まず作業ディレクトリを疑ってください。
> 入力ファイルをどこから実行しても見つけられるようにする方法は 5.5 で紹介します。

## 5.5 実践: CSVファイルの読み込みと集計

実験データの処理はエンジニアの日常業務です。カンマ区切りの行を分解するには
`std::stringstream` と `std::getline` の区切り文字指定を組み合わせます。

サンプルデータ `data/sensor.csv`（時刻[s], 温度[℃], 振動加速度[m/s^2]）:

```csv
time,temp,accel
0.0,23.5,0.12
0.1,23.6,0.34
0.2,23.8,1.52
...
```

```cpp
#include <fstream>
#include <iostream>
#include <sstream>    // stringstream
#include <stdexcept>  // runtime_error
#include <string>
#include <vector>

// センサの1回分の計測値（CSV の1行に対応）
struct SensorSample {
    double time;          // [s]
    double temperature;   // [degC]
    double acceleration;  // [m/s^2]
};

std::vector<SensorSample> LoadSensorCsv(const std::string& csv_path) {
    std::ifstream csv_file(csv_path);
    if (!csv_file) throw std::runtime_error("cannot open: " + csv_path);

    std::vector<SensorSample> samples;
    std::string line;
    std::getline(csv_file, line);            // 1行目（ヘッダ）は読み飛ばす

    while (std::getline(csv_file, line)) {
        std::stringstream line_stream(line); // 1行を stream として扱う
        std::string cell;
        SensorSample sample{};

        std::getline(line_stream, cell, ','); // ',' 区切りで1セル読む
        sample.time = std::stod(cell);        // 文字列 → double 変換
        std::getline(line_stream, cell, ',');
        sample.temperature = std::stod(cell);
        std::getline(line_stream, cell, ',');
        sample.acceleration = std::stod(cell);

        samples.push_back(sample);
    }
    return samples;
}
```

CSV の列名は `temp`, `accel` と短く書かれていますが、プログラムの中では
`temperature`, `acceleration` と略さずに書いています。データファイルの書式に引きずられず、
**コードの中では読みやすい名前に付け直す**のがコツです。

> **CSV を編集するときの注意:** `std::stod` は数値に変換できない文字列（空文字列など）を渡すと
> 例外を投げます。CSV の末尾に空行を入れてしまうと、その行で `std::stod("")` となって
> プログラムが異常終了します。データを自分で編集したときは末尾の空行に気をつけてください
> （`if (line.empty()) continue;` で空行を読み飛ばすのも手です）。

読み込んだ後は 5.1〜5.2 の道具で自在に集計できます。

```cpp
// 温度の平均
double temperature_sum = std::accumulate(samples.begin(), samples.end(), 0.0,
    [](double partial_sum, const SensorSample& sample) {
        return partial_sum + sample.temperature;
    });

// 加速度が最大のサンプル（衝撃の瞬間）を探す
auto peak_sample = std::max_element(samples.begin(), samples.end(),
    [](const SensorSample& lhs, const SensorSample& rhs) {
        return lhs.acceleration < rhs.acceleration;
    });
std::cout << "peak: t = " << peak_sample->time
          << " s, accel = " << peak_sample->acceleration << " m/s^2\n";
```

完全なコードは [`examples/ch05_stl/csv_stats.cpp`](../examples/ch05_stl/csv_stats.cpp) にあります。

> **データファイルの場所について**
> サンプルでは CMakeLists.txt の
> `target_compile_definitions(... DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")`
> という設定で、**CSVファイルの絶対パスをコンパイル時に埋め込んでいます**。
> こうすると exe をどこから実行してもファイルが見つかります（詳細は第8章）。

## 5.6 演習

1. `examples/ch05_stl/data/sensor.csv` を読み込み、次を表示するプログラムを書いてください。
   - 温度の平均・最大・最小
   - 加速度の絶対値が 1.0 m/s² を超えた時刻の一覧（`std::abs` は `<cmath>`）
2. 加速度の**標準偏差**（母標準偏差、つまり n で割るもの）を求めてください
   （加速度だけを `std::vector<double>` に取り出せば、第3章演習の関数を再利用できます）。
3. （発展）集計結果を `summary.csv` として書き出し、Excel で開いて確認してください
   （どこに書き出されるかは 5.4 の「作業ディレクトリ」を参照）。

解答例: [`examples/ch05_stl/solutions/`](../examples/ch05_stl/solutions/)

次章: [第6章 RAIIとスマートポインタ](06_raii_smart_ptr.md)
