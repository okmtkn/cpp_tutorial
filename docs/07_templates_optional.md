# 第7章 テンプレート入門と std::optional

`std::vector<int>` の `<int>` の正体、**テンプレート**を学びます。
テンプレートは「型を後から決められる関数・クラス」を作る仕組みで、
STL 全体を支える土台です。あわせて、「値がないかもしれない」を型で表す
`std::optional` も学びます。

サンプルコード: [`examples/ch07_templates/`](../examples/ch07_templates/)

## 7.1 関数テンプレート

int 版と double 版で同じ中身の関数を2回書くのは無駄です（第3章のオーバーロード）。
テンプレートなら1回で済みます。

```cpp
#include <iostream>
#include <string>

// 「任意の型 T について max_of を定義する」と読む
template <typename T>
T max_of(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    std::cout << max_of(3, 7) << "\n";          // T = int と推論される
    std::cout << max_of(2.5, 1.4) << "\n";      // T = double
    std::cout << max_of(std::string("abc"),
                        std::string("abd")) << "\n";  // T = string（辞書順比較）
    return 0;
}
```

- コンパイラが呼び出し時の引数から `T` を推論し、**型ごとに専用の関数を自動生成**します。
- `>` で比較できない型を渡せばコンパイルエラーになります。
  「その型で中身が成立するなら何でも受け付ける」のがテンプレートです。

## 7.2 実用例: コンテナに使えるテンプレート

```cpp
#include <iostream>
#include <vector>

// どんな型の vector でも合計できる…わけではなく「+= できる型」なら合計できる
template <typename T>
T sum(const std::vector<T>& v) {
    T total{};              // T{} は「その型のゼロ値」(0, 0.0, 空文字列...)
    for (const T& x : v) {
        total += x;
    }
    return total;
}

int main() {
    std::vector<int> a = {1, 2, 3};
    std::vector<double> b = {0.5, 0.25};
    std::vector<std::string> c = {"ab", "cd", "ef"};

    std::cout << sum(a) << "\n";   // 6
    std::cout << sum(b) << "\n";   // 0.75
    std::cout << sum(c) << "\n";   // abcdef （+= が連結だから！）
    return 0;
}
```

## 7.3 クラステンプレート

クラスも型を引数に取れます。`std::vector<T>` や `std::pair<A, B>` の仕組みです。

```cpp
#include <iostream>

// 測定値と単位をセットで持つ型
template <typename T>
struct Measured {
    T value;
    std::string unit;

    void print() const {
        std::cout << value << " [" << unit << "]\n";
    }
};

int main() {
    Measured<double> temp{23.5, "degC"};
    Measured<int>    count{42, "cycles"};
    temp.print();
    count.print();
    return 0;
}
```

> **どこまで学ぶべきか**
> テンプレートは沼のように深い機能ですが、機械系エンジニアとしてまず必要なのは
> **「テンプレートを読める・STLを使える」**ことです。凝ったテンプレートを
> 自作するのは、必要になってからで十分です。

## 7.4 std::optional — 「値がないかもしれない」を型で表す

「検索したが見つからなかった」「計算できなかった」を表すのに、Cでは
`-1` を返す・NULLを返す・エラーコード引数など、場当たり的な方法を使っていました。
C++17 の `std::optional<T>` は「**T の値があるか、空**」を1つの型で表します。

```cpp
#include <iostream>
#include <map>
#include <optional>
#include <string>

// 材料名からヤング率を検索。見つからないかもしれないので optional で返す
std::optional<double> find_young_modulus(const std::string& name) {
    static const std::map<std::string, double> table{
        {"steel", 206.0}, {"aluminum", 70.3}, {"copper", 120.0},
    };
    auto it = table.find(name);
    if (it == table.end()) {
        return std::nullopt;    // 「値なし」を返す
    }
    return it->second;          // 値を返す
}

int main() {
    for (const std::string name : {"steel", "rubber"}) {
        std::optional<double> E = find_young_modulus(name);
        if (E.has_value()) {                 // if (E) とも書ける
            std::cout << name << ": " << *E << " GPa\n";   // * で値を取り出す
        } else {
            std::cout << name << ": 未登録\n";
        }
    }

    // 見つからなければ既定値を使う、が1行で書ける
    double E = find_young_modulus("rubber").value_or(0.01);
    std::cout << "E = " << E << " GPa\n";
    return 0;
}
```

`-1` を返す方式と違い、**呼び出し側が「値がないケース」の処理を忘れるとコンパイル段階で
気づける**（`*E` する前に必ずチェックが要る設計にできる）のが利点です。

## 7.5 例外処理との使い分け

もう1つのエラー処理手段が**例外**です（第4章の `throw` で登場済み）。

```cpp
#include <stdexcept>

double divide(double a, double b) {
    if (b == 0.0) {
        throw std::invalid_argument("zero division");
    }
    return a / b;
}

int main() {
    try {
        double r = divide(1.0, 0.0);
    } catch (const std::exception& e) {
        std::cerr << "エラー: " << e.what() << "\n";
        return 1;
    }
}
```

使い分けの目安:

| 状況 | 手段 |
|------|------|
| 「なくても普通」な失敗（検索ヒットなし等） | `std::optional` |
| 「起きたら異常」な失敗（ファイルが開けない、不正な引数） | 例外 (`throw`) |

## 7.6 演習

1. **線形補間テンプレート:** `template <typename T> T lerp(T a, T b, double t)`
   を作ってください（`a + t * (b - a)` を返す。t=0 で a、t=1 で b）。
   double と第4章の Vec2 の両方で動くことを確認しましょう
   （Vec2 で動かすには何の演算子が必要か考えてください）。
2. **安全な検索:** `std::vector<double>` と目標値 x を受け取り、
   「x 以上になる最初の要素の添字」を `std::optional<std::size_t>` で返す関数を
   書いてください（見つからなければ nullopt）。
3. （発展）5章のCSVデータで「時刻 t のときの温度」を線形補間で求める関数
   `std::optional<double> temp_at(const std::vector<Sample>&, double t)` を
   作ってください（t がデータ範囲外なら nullopt）。

解答例: [`examples/ch07_templates/solutions/`](../examples/ch07_templates/solutions/)

次章: [第8章 CMake実践](08_cmake_project.md)
