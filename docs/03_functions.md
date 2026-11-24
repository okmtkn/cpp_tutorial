# 第3章 関数の進化

C++ の関数はCの関数の上位互換です。この章では、関数まわりでC++が追加した
便利機能と、**「大きなデータは const 参照で渡す」** というC++の基本作法を学びます。

サンプルコード: [`examples/ch03_functions/`](../examples/ch03_functions/)

## 3.1 関数オーバーロード

Cでは同じ名前の関数は1つしか作れませんでした（`abs`, `fabs`, `labs`...と使い分けが必要）。
C++ では**引数の型や個数が違えば同じ名前の関数を複数定義できます**。

```cpp
#include <iostream>

int Square(int value)       { return value * value; }
double Square(double value) { return value * value; }

int main() {
    std::cout << Square(3) << "\n";     // int版が呼ばれる → 9
    std::cout << Square(1.5) << "\n";   // double版が呼ばれる → 2.25
    return 0;
}
```

コンパイラが引数の型を見て自動で適切な方を選びます。

> 自作の関数名は 2.8 の命名規則に従い、`Square` のように単語の頭を大文字にします。
> 関数名は「何をするか」が分かる動詞や名詞（`Square`, `Displacement`, `ScaleAll`）にしましょう。

## 3.2 デフォルト引数

引数に既定値を持たせられます。呼び出し側は省略可能になります。

```cpp
#include <iostream>

constexpr double kStandardGravity = 9.80665;  // 標準重力加速度 [m/s^2]
constexpr double kMoonGravity = 1.62;         // 月面の重力加速度 [m/s^2]

// 等加速度運動の変位 x = v0*t + (1/2)*a*t^2 （加速度を省略すると重力加速度）
double Displacement(double initial_velocity, double elapsed_time,
                    double acceleration = kStandardGravity) {
    return initial_velocity * elapsed_time +
           0.5 * acceleration * elapsed_time * elapsed_time;
}

int main() {
    std::cout << Displacement(0.0, 2.0) << "\n";                // kStandardGravity が使われる
    std::cout << Displacement(0.0, 2.0, kMoonGravity) << "\n";  // 月面
    return 0;
}
```

引数名を `v0, t, a` ではなく `initial_velocity, elapsed_time, acceleration` にしておくと、
関数の宣言を見ただけで「何をどの順に渡せばよいか」が分かります。

デフォルト値は**右側の引数から順に**しか設定できません。

## 3.3 値渡し・参照渡し・const参照渡し

C++ の引数の渡し方は実質この3パターンです。使い分けが重要です。

```cpp
#include <iostream>
#include <string>
#include <vector>

// (1) 値渡し: コピーが渡る。int や double などの小さい型はこれでよい
double Half(double value) { return value / 2.0; }

// (2) 参照渡し: 呼び出し元の変数を書き換えたいとき
void ScaleAll(std::vector<double>& values, double factor) {
    for (double& value : values) value *= factor;
}

// (3) const参照渡し: 大きなデータを「読むだけ」のとき（コピーを避ける）
double Sum(const std::vector<double>& values) {
    double total = 0.0;
    for (double value : values) total += value;
    return total;
}

int main() {
    std::vector<double> lengths = {1.0, 2.0, 3.0};  // 部材の長さ [cm]
    ScaleAll(lengths, 10.0);                  // cm → mm 換算。lengths が {10, 20, 30} になる
    std::cout << Sum(lengths) << "\n";        // 60
    std::cout << Half(Sum(lengths)) << "\n";  // 30
    return 0;
}
```

| 渡し方 | 書き方 | 使いどころ |
|--------|--------|-----------|
| 値渡し | `double value` | int, double など小さい型 |
| 参照渡し | `std::vector<double>& values` | 呼び出し元のデータを**書き換える** |
| const参照渡し | `const std::vector<double>& values` | 大きなデータを**読むだけ**（コピー回避） |

> `ScaleAll` や `Sum` は「どんな数列にも使える汎用の関数」なので、引数名も
> `values`（値の列）という汎用の名前にしています。一方、呼び出す側の `main` では
> `lengths` のように**具体的な中身を表す名前**を付けます。

> **なぜ const 参照？**
> `std::vector` や `std::string` を値渡しすると、呼び出しのたびに全要素がコピーされます。
> 要素数が100万個なら100万個コピーです。const参照なら別名を渡すだけなのでコストゼロ。
> 「**vector や string を受け取る引数はとりあえず `const &`**」と覚えてください。

## 3.4 vector を返す関数

Cで「配列を返す」のは面倒でした（ポインタと長さを引数で返す等）。
C++ では **vector をそのまま return できます**（コンパイラが効率よく処理してくれます）。

```cpp
#include <cmath>     // std::round
#include <iostream>
#include <vector>

// 0 から end_time まで time_step 刻みの時刻列を作る（シミュレーションの時間軸など）
std::vector<double> MakeTimePoints(double end_time, double time_step) {
    // 刻みの回数を先に整数で求めておく（理由は下の注意を参照）
    const int num_steps = static_cast<int>(std::round(end_time / time_step));
    std::vector<double> time_points;
    for (int i = 0; i <= num_steps; ++i) {
        time_points.push_back(i * time_step);
    }
    return time_points;   // これでOK
}

int main() {
    auto time_points = MakeTimePoints(1.0, 0.25);
    for (double time : time_points) std::cout << time << " ";   // 0 0.25 0.5 0.75 1
    std::cout << "\n";
    return 0;
}
```

> **注意: 小数を足し続けるループは避ける**
> `for (double time = 0.0; time <= end_time; time += time_step)` と書きたくなりますが、
> 0.1 のような値は2進数で正確に表せないため、足し続けると誤差がたまり、
> 最後の時刻 `end_time` が**入ったり入らなかったり**します（0.1 を10回足しても 1.0 ぴったりにはなりません）。
> そこで「何回刻むか」を整数 `num_steps` で先に決め、時刻は `i * time_step` で毎回計算します。
> 数値シミュレーションでは必ず身につけたい書き方です（第10章でも使います）。
>
> `static_cast<int>(...)` は、Cの `(int)(...)` と同じ**型変換（キャスト）**のC++での書き方です。
> `std::round` で四捨五入してから int に変換しているので、`4.9999...` が `4` に切り捨てられる心配もありません。

## 3.5 複数の値を返す — pair と構造化束縛

Cでは複数の戻り値を返すのにポインタ引数を使っていました。
C++17 では `std::pair`（2値の組）と**構造化束縛**でスッキリ書けます。

```cpp
#include <iostream>
#include <utility>   // std::pair
#include <vector>

// 最小値と最大値を同時に返す
std::pair<double, double> MinMax(const std::vector<double>& values) {
    double min_value = values[0];
    double max_value = values[0];
    for (double value : values) {
        if (value < min_value) min_value = value;
        if (value > max_value) max_value = value;
    }
    return {min_value, max_value};   // pair を作って返す
}

int main() {
    std::vector<double> stresses = {120.5, 98.2, 141.7, 110.3};  // 応力データ [MPa]

    // 構造化束縛: 2つの変数に一度に受け取る
    auto [min_stress, max_stress] = MinMax(stresses);
    std::cout << "min: " << min_stress << " MPa, max: " << max_stress << " MPa\n";
    return 0;
}
```

なお `MinMax` は `values[0]` を読むので、**空の vector を渡してはいけません**（範囲外アクセスになります）。
このような「関数を呼ぶ側が守るべき前提」はコメントに書いておくと親切です。

`auto [min_stress, max_stress] = ...` が構造化束縛です。受け取る側で
「何の最小・最大か」が分かる名前を自由に付けられるのも利点です。3つ以上返したい場合は `std::tuple`、
または第4章で学ぶ**構造体/クラスを返す**のが一般的です。

## 3.6 constexpr 関数（コンパイル時計算）

`constexpr` を付けた関数は、コンパイル時に計算**できる**関数です。
下の `kRightAngleRad` のように、結果を `constexpr` 変数に入れるなど「コンパイル時に値が必要な場面」で
定数を渡して呼ぶと、コンパイル時に計算されます（普通の変数を渡せば、普通の関数として実行時に計算されます）。

```cpp
constexpr double kPi = 3.14159265358979323846;

constexpr double DegToRad(double degrees) {
    return degrees * kPi / 180.0;
}

constexpr double kRightAngleRad = DegToRad(90.0);   // コンパイル時に計算済み
```

「マクロで計算式を書いていた」場面の置き換えです。まずは
「`#define` の代わりに `constexpr` がある」程度の理解で十分です。

## 3.7 演習

1. **台形則による数値積分:** 関数
   `double IntegrateSinTrapezoid(double lower_limit, double upper_limit, int num_intervals)` を作り、
   区間 [lower_limit, upper_limit] を num_intervals 分割した台形則で f(x) = sin(x) を積分してください。
   `[0, π]` での理論値は 2.0 です。分割数 10, 100, 1000 で誤差がどう減るか表示しましょう。
   （`#include <cmath>` で `std::sin` が使えます。π は `M_PI` ではなく
   自分で `constexpr double kPi = 3.14159...;` と定義するのが移植性の高い書き方です）
2. **統計関数:** `std::pair<double, double> MeanAndStddev(const std::vector<double>& values)`
   を作り、平均と標準偏差を構造化束縛で受け取って表示してください。
   標準偏差は**母標準偏差**（偏差の2乗和を要素数 n で割って平方根をとるもの）とします。
3. （発展）1. の IntegrateSinTrapezoid をオーバーロードして、
   `IntegrateSinTrapezoid(double lower_limit, double upper_limit)` （1000 分割のデフォルト動作）も作ってみましょう。
   デフォルト引数で書く方法とオーバーロードで書く方法、どちらが良いか考えてみてください。

解答例: [`examples/ch03_functions/solutions/`](../examples/ch03_functions/solutions/)

次章: [第4章 クラス](04_classes.md)
