# 第7章 テンプレート入門と std::optional

この章のテーマは一言でいうと **「型だけ違う同じ処理を、1回書けば済むようにする」** です。
「テンプレート」という言葉は難しそうですが、困りごとは身近なものなので、そこから始めます。

サンプルコード: [`examples/ch07_templates/`](../examples/ch07_templates/)

### まず、何が困るのか

第3章のオーバーロードでは、2乗を計算する関数を型ごとに書きました。

```cpp
int Square(int value)       { return value * value; }
double Square(double value) { return value * value; }
```

ここで float 版や long 版も欲しくなったらどうでしょう。
**中身はまったく同じ**なのに、型の部分だけ書き換えたコピーがどんどん増えていきます。
しかも、あとで処理を直したくなったら、コピーを全部同じように直さなければなりません。
1つでも直し忘れると、「int のときだけ結果がおかしい」というバグになります。

Cではこういうとき、マクロ `#define SQUARE(x) ((x) * (x))` で逃げることがありました。
しかしマクロは単なる文字の置き換えなので、型のチェックが効かず、
`SQUARE(i++)` のように書くと `i` が2回増えるといった落とし穴があります。

### C++の答え: 型の部分を「空欄」にした型紙を作る

板金加工の**型紙**を思い出してください。形は型紙で決まっていて、
鉄板を当てれば鉄の部品が、アルミ板を当てればアルミの部品ができます。

C++の**テンプレート**も同じ考え方です。
処理の中身は1回だけ書き、**型の部分を `T` という空欄にしておきます**。
`int` で呼べば int 版、`double` で呼べば double 版の関数を、コンパイラが自動で作ってくれます。

実はみなさんは、もうテンプレートを使っています。
`std::vector<double>` の `<double>` は、「vector という型紙の空欄に double を当てはめる」
という意味だったのです。

この章では次の順に学びます。

1. **テンプレート（7.1〜7.3）** … 型紙になる関数・クラスの作り方
2. **`std::optional`（7.4〜7.5）** … 「検索したが見つからなかった」を、Cのように `-1` などの特別な値で
   ごまかさずに表す型。これもテンプレートで作られています

## 7.1 関数テンプレート

導入の `Square` と同じく、2つの値の大きい方を返す関数を int 版・double 版と2回書くのは無駄です。
テンプレートなら1回で済みます。

```cpp
#include <iostream>
#include <string>

// 「任意の型 T について MaxOf を定義する」と読む
template <typename T>
T MaxOf(T lhs, T rhs) {
    return (lhs > rhs) ? lhs : rhs;
}

int main() {
    std::cout << MaxOf(3, 7) << "\n";          // T = int と推論される
    std::cout << MaxOf(2.5, 1.4) << "\n";      // T = double
    std::cout << MaxOf(std::string("abc"),
                       std::string("abd")) << "\n";  // T = string（辞書順比較）
    return 0;
}
```

> テンプレートの型引数 `T` は「どんな型でもよい」ことを表す**慣習的な名前**なので、1文字のままで構いません。

- コンパイラが呼び出し時の引数から `T` を推論し、**型ごとに専用の関数を自動生成**します。
- `>` で比較できない型を渡せばコンパイルエラーになります。
  「その型で中身が成立するなら何でも受け付ける」のがテンプレートです。

> **つまずきやすい点**
> - `MaxOf(3, 2.5)` のように**型の違う2つ**を渡すと、`T` を int にするか double にするか決められず
>   コンパイルエラーになります。`MaxOf(3.0, 2.5)` と型を揃えるか、`MaxOf<double>(3, 2.5)` と `T` を明示します。
> - 上で `std::string("abc")` とわざわざ包んでいるのは、`MaxOf("abc", "abd")` と書くと
>   `T` が文字列ポインタ（`const char*`）になり、**文字列の中身ではなくアドレスを比較してしまう**からです
>   （コンパイルは通るのに結果がでたらめ、という厄介な間違いです）。

## 7.2 実用例: コンテナに使えるテンプレート

```cpp
#include <iostream>
#include <string>
#include <vector>

// どんな型の vector でも合計できる…わけではなく「+= できる型」なら合計できる
template <typename T>
T Sum(const std::vector<T>& values) {
    T total{};              // T{} は「その型のゼロ値」(0, 0.0, 空文字列...)
    for (const T& value : values) {
        total += value;
    }
    return total;
}

int main() {
    std::vector<int> daily_part_counts = {1, 2, 3};             // 日ごとの部品数 [個]
    std::vector<double> weight_masses = {0.5, 0.25};            // おもりの質量 [kg]
    std::vector<std::string> code_parts = {"ab", "cd", "ef"};   // 部品コードの断片

    std::cout << Sum(daily_part_counts) << "\n";   // 6
    std::cout << Sum(weight_masses) << "\n";       // 0.75
    std::cout << Sum(code_parts) << "\n";          // abcdef （+= が連結だから！）
    return 0;
}
```

## 7.3 クラステンプレート

クラスも型を引数に取れます。`std::vector<T>` や `std::pair<A, B>` の仕組みです。

```cpp
#include <iostream>
#include <string>

// 測定値と単位をセットで持つ型
template <typename T>
struct Measured {
    T value;
    std::string unit;

    void Print() const {
        std::cout << value << " [" << unit << "]\n";
    }
};

int main() {
    Measured<double> room_temperature{23.5, "degC"};
    Measured<int>    load_cycle_count{42, "cycles"};   // 疲労試験の負荷サイクル数
    room_temperature.Print();
    load_cycle_count.Print();
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
std::optional<double> FindYoungModulus(const std::string& material) {
    // static な定数（プログラム終了まで残る）は k + PascalCase で名付ける
    static const std::map<std::string, double> kYoungModulusTable{
        {"steel", 206.0}, {"aluminum", 70.3}, {"copper", 120.0},
    };
    auto entry = kYoungModulusTable.find(material);
    if (entry == kYoungModulusTable.end()) {
        return std::nullopt;    // 「値なし」を返す
    }
    return entry->second;       // 値を返す
}

int main() {
    for (const std::string material : {"steel", "rubber"}) {
        std::optional<double> young_modulus = FindYoungModulus(material);
        if (young_modulus.has_value()) {     // if (young_modulus) とも書ける
            std::cout << material << ": " << *young_modulus << " GPa\n";   // * で値を取り出す
        } else {
            std::cout << material << ": not registered\n";
        }
    }

    // 見つからなければ既定値を使う、が1行で書ける
    double rubber_modulus = FindYoungModulus("rubber").value_or(0.01);
    std::cout << "rubber (default) = " << rubber_modulus << " GPa\n";
    return 0;
}
```

`-1` を返す方式と違い、**関数の宣言（戻り値の型）を見ただけで「値がないことがある」と分かる**のが利点です。
`-1` が「見つからなかった」の意味なのか本当の値なのかを、ドキュメントで確認する必要がありません。

> **注意: チェックを忘れてもコンパイルエラーにはならない**
> 空の optional に `*young_modulus` を使っても、コンパイルは通ってしまい、実行時に
> でたらめな値になります（未定義動作）。`*` を使う前には必ず `if (young_modulus)` で確認してください。
> 確認を忘れたときに気づきたい場合は、`*` の代わりに `young_modulus.value()` を使うと、
> 空のときに例外（`std::bad_optional_access`）が投げられて異常に気づけます。

> 数式ではヤング率を `E` と書きますが、C++ のコードでは `young_modulus` と書きます。
> 1文字の記号は式の中では便利でも、コードの中では「何の E か」が伝わらないからです。

## 7.5 例外処理との使い分け

もう1つのエラー処理手段が**例外**です（第4章の `throw` で登場済み）。

```cpp
#include <iostream>
#include <stdexcept>

double Divide(double numerator, double denominator) {
    if (denominator == 0.0) {
        throw std::invalid_argument("zero division");
    }
    return numerator / denominator;
}

int main() {
    try {
        double quotient = Divide(1.0, 0.0);
        std::cout << quotient << "\n";
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << "\n";
        return 1;
    }
    return 0;
}
```

例外の流れを整理しておきます。

1. `throw` が実行されると、その関数の残りの処理は**実行されずに**打ち切られる
   （上の例では `return numerator / denominator;` も、`main` の `std::cout << quotient` も実行されない）
2. 呼び出し元を1段ずつさかのぼり、最初に見つかった `try { }` に対応する `catch` に飛ぶ。
   途中で抜けた `{ }` のローカル変数は、第6章の RAII のとおりデストラクタで後片付けされる
3. `catch (const std::exception& error)` は、`std::invalid_argument` や `std::runtime_error` など
   標準ライブラリの例外をまとめて受け取れる書き方。`error.what()` で `throw` 時のメッセージを取り出せる
4. **どこにも `catch` が無ければ、プログラムは異常終了する**

使い分けの目安:

| 状況 | 手段 |
|------|------|
| 「なくても普通」な失敗（検索ヒットなし等） | `std::optional` |
| 「起きたら異常」な失敗（ファイルが開けない、不正な引数） | 例外 (`throw`) |

## 7.6 演習

1. **線形補間テンプレート:** `template <typename T> T Lerp(T start, T end, double ratio)`
   を作ってください（`start + ratio * (end - start)` を返す。ratio=0 で start、ratio=1 で end）。
   double と第4章の Vec2 の両方で動くことを確認しましょう
   （Vec2 で動かすには何の演算子が必要か考えてください）。
2. **安全な検索:** `std::vector<double>` としきい値 threshold を受け取り、
   「threshold 以上になる最初の要素の添字」を `std::optional<std::size_t>` で返す関数
   `FindFirstAtLeast` を書いてください（見つからなければ nullopt）。
3. （発展）5章の CSV のように「時刻と温度」が時刻順に並んだデータから、
   「時刻 query_time のときの温度」を線形補間で求める関数
   `std::optional<double> TemperatureAt(const std::vector<TemperatureSample>& samples, double query_time)` を
   作ってください（query_time がデータ範囲外なら nullopt）。
   `TemperatureSample` は `struct TemperatureSample { double time; double temperature; };` として
   自分で定義し、データは数点を直接書き込めば十分です
   （余力があれば 5章の `LoadSensorCsv` で読んだ `SensorSample` を使う形に変えてみましょう）。

解答例: [`examples/ch07_templates/solutions/`](../examples/ch07_templates/solutions/)

次章: [第8章 CMake実践](08_cmake_project.md)
