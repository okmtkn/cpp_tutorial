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
    std::vector<double> data = {23.5, 21.2, 25.8, 22.1, 24.9};

    // 合計と平均
    double total = std::accumulate(data.begin(), data.end(), 0.0);
    std::cout << "平均: " << total / data.size() << "\n";

    // 最大・最小（イテレータが返るので * で値を取り出す）
    double hi = *std::max_element(data.begin(), data.end());
    double lo = *std::min_element(data.begin(), data.end());
    std::cout << "最大: " << hi << " 最小: " << lo << "\n";

    // 昇順ソート
    std::sort(data.begin(), data.end());
    for (double x : data) std::cout << x << " ";
    std::cout << "\n";
    return 0;
}
```

- `data.begin(), data.end()` は「コンテナの先頭から末尾まで」という範囲指定の
  お約束です。ほとんどの algorithm 関数がこの形で範囲を受け取ります。
- `std::accumulate` の第3引数 `0.0` は合計の初期値です。
  **`0`（int）と書くと小数が切り捨てられるバグ**になるので注意！（典型的なハマりどころ）

## 5.2 ラムダ式 — その場で作る小さな関数

「降順にソートしたい」「条件を満たす要素を数えたい」など、
algorithm に**判定基準を渡したい**場面で使うのが**ラムダ式**です。

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<double> stress = {120.5, 98.2, 141.7, 110.3, 135.2};

    // 降順ソート: 比較関数をラムダ式で渡す
    std::sort(stress.begin(), stress.end(),
              [](double a, double b) { return a > b; });

    // 条件を満たす要素数: 120 MPa を超えるデータはいくつ？
    int n = std::count_if(stress.begin(), stress.end(),
                          [](double x) { return x > 120.0; });
    std::cout << "120MPa超え: " << n << " 件\n";

    // 変数を「捕獲」して使う: しきい値を変数にする
    double limit = 130.0;
    int m = std::count_if(stress.begin(), stress.end(),
                          [limit](double x) { return x > limit; });
    std::cout << limit << "MPa超え: " << m << " 件\n";
    return 0;
}
```

ラムダ式の文法:

```
[捕獲リスト](引数) { 本体 }
```

- `[]` … 外の変数を使わない
- `[limit]` … 変数 limit をコピーして使う
- `[&]` … 外の変数をすべて参照で使う（短いラムダで便利だが多用注意）

「名前を付けるほどでもない使い捨ての関数」をその場に書ける機能、と理解してください。

## 5.3 std::map — キーと値の対応表

`std::map<キーの型, 値の型>` は辞書（対応表）です。

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    // 材料名 → ヤング率 [GPa]
    std::map<std::string, double> young_modulus{
        {"steel", 206.0},
        {"aluminum", 70.3},
        {"copper", 120.0},
    };

    young_modulus["titanium"] = 106.0;   // 追加も代入も []

    std::cout << young_modulus["steel"] << " GPa\n";

    // 全要素を回す（キーの昇順に並ぶ）。構造化束縛が活躍
    for (const auto& [name, E] : young_modulus) {
        std::cout << name << ": " << E << " GPa\n";
    }

    // 存在チェックは count() か find() で
    if (young_modulus.count("concrete") == 0) {
        std::cout << "concrete は未登録です\n";
    }
    return 0;
}
```

> **注意:** `map[キー]` は**キーが存在しないと値0で勝手に登録**します。
> 「読むだけ」のつもりのときは `at()` か `find()` を使ってください。

## 5.4 ファイル入出力 — fstream

Cの `fopen`/`fclose` に相当するのが `std::ifstream`（読み）/`std::ofstream`（書き）です。
**close は自動で呼ばれる**のがポイントです（この仕組み＝RAIIは第6章で解説）。

```cpp
#include <fstream>
#include <iostream>
#include <string>

int main() {
    // 書き込み
    std::ofstream out("result.txt");
    out << "x, x^2\n";
    for (int x = 1; x <= 5; ++x) {
        out << x << ", " << x * x << "\n";
    }
    // out.close() は書かなくてよい（スコープを抜けると自動で閉じる）

    // 読み込み（1行ずつ）
    std::ifstream in("result.txt");
    if (!in) {   // 開けたかどうかのチェックは必須！
        std::cerr << "ファイルが開けません\n";
        return 1;
    }
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << "\n";
    }
    return 0;
}
```

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
#include <sstream>   // stringstream
#include <string>
#include <vector>

struct Sample {
    double time;
    double temp;
    double accel;
};

std::vector<Sample> load_csv(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open: " + path);

    std::vector<Sample> samples;
    std::string line;
    std::getline(in, line);              // 1行目（ヘッダ）は読み飛ばす

    while (std::getline(in, line)) {
        std::stringstream ss(line);      // 1行を stream として扱う
        std::string cell;
        Sample s{};

        std::getline(ss, cell, ',');     // ',' 区切りで1セル読む
        s.time = std::stod(cell);        // 文字列 → double 変換
        std::getline(ss, cell, ',');
        s.temp = std::stod(cell);
        std::getline(ss, cell, ',');
        s.accel = std::stod(cell);

        samples.push_back(s);
    }
    return samples;
}
```

読み込んだ後は 5.1〜5.2 の道具で自在に集計できます。

```cpp
// 温度の平均
double t_sum = std::accumulate(samples.begin(), samples.end(), 0.0,
    [](double acc, const Sample& s) { return acc + s.temp; });

// 加速度が最大のサンプル（衝撃の瞬間）を探す
auto peak = std::max_element(samples.begin(), samples.end(),
    [](const Sample& a, const Sample& b) { return a.accel < b.accel; });
std::cout << "ピーク: t=" << peak->time << "s, " << peak->accel << " m/s^2\n";
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
2. 加速度の**標準偏差**を求めてください（第3章演習の関数を再利用できます）。
3. （発展）集計結果を `summary.csv` として書き出し、Excel で開いて確認してください。

解答例: [`examples/ch05_stl/solutions/`](../examples/ch05_stl/solutions/)

次章: [第6章 RAIIとスマートポインタ](06_raii_smart_ptr.md)
