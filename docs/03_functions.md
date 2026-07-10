# 第3章 関数の進化

C++ の関数はCの関数の上位互換です。この章では、関数まわりでC++が追加した
便利機能と、**「大きなデータは const 参照で渡す」** というC++の基本作法を学びます。

サンプルコード: [`examples/ch03_functions/`](../examples/ch03_functions/)

## 3.1 関数オーバーロード

Cでは同じ名前の関数は1つしか作れませんでした（`abs`, `fabs`, `labs`...と使い分けが必要）。
C++ では**引数の型や個数が違えば同じ名前の関数を複数定義できます**。

```cpp
#include <iostream>

int square(int x)       { return x * x; }
double square(double x) { return x * x; }

int main() {
    std::cout << square(3) << "\n";     // int版が呼ばれる → 9
    std::cout << square(1.5) << "\n";   // double版が呼ばれる → 2.25
    return 0;
}
```

コンパイラが引数の型を見て自動で適切な方を選びます。

## 3.2 デフォルト引数

引数に既定値を持たせられます。呼び出し側は省略可能になります。

```cpp
#include <iostream>

// 等加速度運動の変位 x = v0*t + (1/2)*a*t^2 （a を省略すると重力加速度）
double displacement(double v0, double t, double a = 9.80665) {
    return v0 * t + 0.5 * a * t * t;
}

int main() {
    std::cout << displacement(0.0, 2.0) << "\n";        // a = 9.80665 が使われる
    std::cout << displacement(0.0, 2.0, 1.62) << "\n";  // 月面 (a = 1.62)
    return 0;
}
```

デフォルト値は**右側の引数から順に**しか設定できません。

## 3.3 値渡し・参照渡し・const参照渡し

C++ の引数の渡し方は実質この3パターンです。使い分けが重要です。

```cpp
#include <iostream>
#include <string>
#include <vector>

// (1) 値渡し: コピーが渡る。int や double などの小さい型はこれでよい
double half(double x) { return x / 2.0; }

// (2) 参照渡し: 呼び出し元の変数を書き換えたいとき
void scale(std::vector<double>& v, double factor) {
    for (double& x : v) x *= factor;
}

// (3) const参照渡し: 大きなデータを「読むだけ」のとき（コピーを避ける）
double sum(const std::vector<double>& v) {
    double s = 0.0;
    for (double x : v) s += x;
    return s;
}

int main() {
    std::vector<double> data = {1.0, 2.0, 3.0};
    scale(data, 10.0);                     // data が {10, 20, 30} になる
    std::cout << sum(data) << "\n";        // 60
    std::cout << half(sum(data)) << "\n";  // 30
    return 0;
}
```

| 渡し方 | 書き方 | 使いどころ |
|--------|--------|-----------|
| 値渡し | `int x` | int, double など小さい型 |
| 参照渡し | `std::vector<double>& v` | 呼び出し元のデータを**書き換える** |
| const参照渡し | `const std::vector<double>& v` | 大きなデータを**読むだけ**（コピー回避） |

> **なぜ const 参照？**
> `std::vector` や `std::string` を値渡しすると、呼び出しのたびに全要素がコピーされます。
> 要素数が100万個なら100万個コピーです。const参照なら別名を渡すだけなのでコストゼロ。
> 「**vector や string を受け取る引数はとりあえず `const &`**」と覚えてください。

## 3.4 vector を返す関数

Cで「配列を返す」のは面倒でした（ポインタと長さを引数で返す等）。
C++ では **vector をそのまま return できます**（コンパイラが効率よく処理してくれます）。

```cpp
#include <iostream>
#include <vector>

// 0 から end まで step 刻みの数列を作る（時間軸の生成などでよく使う）
std::vector<double> linspace_step(double end, double step) {
    std::vector<double> result;
    for (double x = 0.0; x <= end; x += step) {
        result.push_back(x);
    }
    return result;   // これでOK
}

int main() {
    auto t = linspace_step(1.0, 0.25);
    for (double x : t) std::cout << x << " ";   // 0 0.25 0.5 0.75 1
    std::cout << "\n";
    return 0;
}
```

## 3.5 複数の値を返す — pair と構造化束縛

Cでは複数の戻り値を返すのにポインタ引数を使っていました。
C++17 では `std::pair`（2値の組）と**構造化束縛**でスッキリ書けます。

```cpp
#include <iostream>
#include <utility>   // std::pair
#include <vector>

// 最小値と最大値を同時に返す
std::pair<double, double> min_max(const std::vector<double>& v) {
    double lo = v[0], hi = v[0];
    for (double x : v) {
        if (x < lo) lo = x;
        if (x > hi) hi = x;
    }
    return {lo, hi};   // pair を作って返す
}

int main() {
    std::vector<double> stress = {120.5, 98.2, 141.7, 110.3};  // 応力データ [MPa]

    auto [lo, hi] = min_max(stress);   // 構造化束縛: 2つの変数に一度に受け取る
    std::cout << "最小: " << lo << " MPa, 最大: " << hi << " MPa\n";
    return 0;
}
```

`auto [lo, hi] = ...` が構造化束縛です。3つ以上返したい場合は `std::tuple`、
または第4章で学ぶ**構造体/クラスを返す**のが一般的です。

## 3.6 constexpr 関数（コンパイル時計算）

`constexpr` を付けた関数は、引数が定数ならコンパイル時に計算されます。

```cpp
constexpr double deg_to_rad(double deg) {
    return deg * 3.14159265358979323846 / 180.0;
}

constexpr double angle = deg_to_rad(90.0);   // コンパイル時に計算済み
```

「マクロで計算式を書いていた」場面の置き換えです。まずは
「`#define` の代わりに `constexpr` がある」程度の理解で十分です。

## 3.7 演習

1. **台形則による数値積分:** 関数
   `double trapezoid(double a, double b, int n)` を作り、
   区間 [a, b] を n 分割した台形則で f(x) = sin(x) を積分してください。
   `[0, π]` での理論値は 2.0 です。n = 10, 100, 1000 で誤差がどう減るか表示しましょう。
   （`#include <cmath>` で `std::sin` が使えます。π は `M_PI` ではなく
   自分で `constexpr double PI = 3.14159...;` と定義するのが移植性の高い書き方です）
2. **統計関数:** `std::pair<double, double> mean_stddev(const std::vector<double>& v)`
   を作り、平均と標準偏差を構造化束縛で受け取って表示してください。
3. （発展）1. の trapezoid をオーバーロードして、
   `trapezoid(double a, double b)` （n=1000 のデフォルト動作）も作ってみましょう。
   デフォルト引数で書く方法とオーバーロードで書く方法、どちらが良いか考えてみてください。

解答例: [`examples/ch03_functions/solutions/`](../examples/ch03_functions/solutions/)

次章: [第4章 クラス](04_classes.md)
