# 第2章 CからC++へ

C++ は「Cの上位互換」なので、これまで書いてきたCのコードはほぼそのまま動きます。
しかし、C++ には**より安全で書きやすい道具**が揃っています。
この章では、Cの書き方と対比しながら「最初に置き換えるべき道具」を学びます。

サンプルコード: [`examples/ch02_from_c/`](../examples/ch02_from_c/)

## 2.1 入出力: printf/scanf → cout/cin

```cpp
#include <iostream>  // stdio.h の代わり

int main() {
    int age;
    std::cout << "Enter your age: ";
    std::cin >> age;
    std::cout << "You are " << age << " years old.\n";
    return 0;
}
```

- `std::` は「標準ライブラリの名前空間」です。`std::cout` で「標準ライブラリの cout」という意味になります。
- `<<` で値を流し込むイメージです。**`%d` や `%f` の書式指定子が不要**で、
  型を間違えてバグる事故（`%d` に double を渡す等）が起きません。
- 改行は `"\n"` で書きます（`std::endl` でも改行できますが、`"\n"` で十分です）。

> **`using namespace std;` について**
> `std::` を省略できるようになる宣言ですが、名前の衝突事故の原因になるため、
> このチュートリアルでは使いません。実務でもヘッダファイルに書くのは禁止が一般的です。

## 2.2 文字列: char配列 → std::string

Cの文字列処理は `char buf[64]` のサイズ管理や `strcpy`/`strcat` のバッファオーバーランと
戦う必要がありました。C++ では `std::string` がすべて面倒を見てくれます。

```cpp
#include <iostream>
#include <string>

int main() {
    std::string name = "Kosen Taro";
    std::string greeting = "Hello, " + name + "!";  // + で連結できる

    std::cout << greeting << "\n";
    std::cout << "length: " << greeting.size() << "\n";      // strlen 相当
    std::cout << "first char: " << greeting[0] << "\n";

    if (name == "Kosen Taro") {   // strcmp ではなく == で比較できる
        std::cout << "name matched\n";
    } else {
        std::cout << "name not matched\n";
    }

    std::string line;
    std::cout << "Type one line: ";
    std::getline(std::cin, line);   // 空白を含む1行を読む（gets の安全版）
    std::cout << "you typed: " << line << "\n";
    return 0;
}
```

| Cでの書き方 | C++での書き方 |
|-------------|---------------|
| `char s[64]; strcpy(s, "abc");` | `std::string s = "abc";` |
| `strcat(s, t)` | `s += t` または `s + t` |
| `strlen(s)` | `s.size()` |
| `strcmp(s, t) == 0` | `s == t` |

> **つまずきやすい点**
> - `+` で連結できるのは、どちらかが `std::string` のときだけです。
>   `"Hello, " + "world"` のように**文字列リテラル同士**を足すとコンパイルエラーになります
>   （リテラルは `std::string` ではなく、Cと同じ `char` の配列だからです）。
> - `std::cin >> age;` のあとに `std::getline` を使うと、`>>` が読み残した改行だけを読んで
>   **空の1行**が返ってきます。数値の入力と行の入力を混ぜるときは、
>   `std::cin >> age;` の直後に `std::cin.ignore(1000, '\n');` を書いて残りの改行を捨ててください。
> - `size()` が返すのは**バイト数**です。UTF-8 の日本語は1文字が3バイトなので、文字数とは一致しません。

## 2.3 配列: 固定長配列 → std::vector

`std::vector` は**サイズを自動管理してくれる可変長配列**です。
`malloc`/`free`/`realloc` は不要になります。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> bolt_lengths = {10, 20, 30};   // ボルトの長さ [mm]（int の可変長配列）

    bolt_lengths.push_back(40);       // 末尾に追加（自動でメモリが伸びる）
    bolt_lengths.push_back(50);

    std::cout << "size : " << bolt_lengths.size() << "\n";
    std::cout << "front: " << bolt_lengths[0]
              << ", back: " << bolt_lengths.back() << "\n";

    // 従来の for でも回せる
    for (std::size_t i = 0; i < bolt_lengths.size(); ++i) {
        std::cout << bolt_lengths[i] << " ";
    }
    std::cout << "\n";
    return 0;
}
```

添字の型 `std::size_t` は「要素数や添字を表すための**符号なし**整数型」で、`size()` もこの型を返します。
`int i` で書くと「符号あり（int）と符号なし（size_t）を比べている」という警告が
`-Wextra` で出るので、`size()` と比べる添字には `std::size_t` を使います。
符号なしなので**0 未満にはならない**（0 から 1 を引くと巨大な数になる）点に注意してください。

`std::vector<int>` の `<int>` は「int を入れる vector」という意味です。
`std::vector<double>` や `std::vector<std::string>` も同じように作れます
（この仕組み＝テンプレートは第7章で学びます）。

## 2.4 範囲for文 — 「全要素を順に処理」の決定版

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<double> temperatures = {23.5, 24.1, 22.8, 25.0};  // [degC]

    // 「temperatures の各要素 temperature について」と読む
    for (double temperature : temperatures) {
        std::cout << temperature << " ";
    }
    std::cout << "\n";

    // 要素を書き換えたいときは参照 & を付ける（2.6参照）
    for (double& temperature : temperatures) {
        temperature += 273.15;   // 摂氏 → 絶対温度
    }

    for (double temperature : temperatures) {
        std::cout << temperature << " ";
    }
    std::cout << "\n";
    return 0;
}
```

添字 `i` を書かないので、**off-by-one エラー（`<=` と `<` の取り違え等）が原理的に起きません**。
「全要素を先頭から順に」処理するときは常に範囲forを使いましょう。

コンテナには複数形（`temperatures`）、1つずつ取り出した要素には単数形（`temperature`）の
名前を付けると、`for (double temperature : temperatures)` が英文のように読めます
（名前の付け方は 2.8 でまとめます）。

## 2.5 型推論: auto

`auto` と書くと、コンパイラが右辺から型を推論してくれます。

```cpp
auto sample_count = 42;                     // int
auto shaft_diameter = 3.14;                 // double
auto material = std::string("steel");       // std::string
auto motor_ids = std::vector<int>{1, 2, 3}; // std::vector<int>

for (auto temperature : temperatures) { ... }   // 範囲forでもよく使う
```

型名が長くなるC++では多用しますが、**「型がわからなくなるまで」使うのはNG**です。
右辺を見て型が明らかなときに使う、が目安です。

> **注意:** `auto material = "steel";` と書くと、`material` は `std::string` ではなく
> Cと同じ文字列ポインタ（`const char*`）になります。上の例で `std::string("steel")` と
> わざわざ書いているのはこのためです。文字列は `std::string material = "steel";` と
> 型を書くのが確実です。

## 2.6 参照 — ポインタの安全な代替

Cで「関数で呼び出し元の変数を書き換える」にはポインタを使いました。
C++ では**参照（reference）**が使えます。参照は「変数の別名」です。

```cpp
#include <iostream>

// C流:  void Increment(int* count) { (*count)++; }  → 呼び出しは Increment(&part_count);
// C++流: 参照で受け取る
void Increment(int& count) {
    count++;   // count は呼び出し元の変数そのもの
}

int main() {
    int part_count = 10;              // 部品の個数
    Increment(part_count);            // & も * も書かない
    std::cout << part_count << "\n";  // 11
    return 0;
}
```

- 参照は**必ず初期化が必要**で、**NULLになれない**ため、ヌルポインタ事故が起きません。
- ポインタが不要になったわけではありませんが（第6章で再登場）、
  「関数に変数を書き換えてもらう」用途は参照で書くのがC++流です。

### 「別名」とはどういうことか

参照は関数の引数以外でも作れます。まずは `main` の中だけで試してみましょう。

> ここから「`&` の2つの意味」までの短い断片では、文法の形そのものを見せるために
> `a`, `r`, `p` のような1文字の名前を使っています。実際のプログラムでは 2.8 で説明する
> 「中身が分かる名前」を付けてください。

```cpp
int a = 10;
int& r = a;     // r は a の別名（「r という名前でも a を呼べる」）

r = 20;         // r に代入すると…
std::cout << a << "\n";   // 20  ← a が変わっている
a = 30;
std::cout << r << "\n";   // 30  ← r から見ても変わっている
```

`r` は `a` のコピーではありません。**同じ1つの箱（メモリ）に名前が2つ付いている**だけです。
あだ名をイメージするとわかりやすいでしょう。「山田太郎くん」を「タロウ」と呼んでも、
同じ1人の人を指しています。タロウに荷物を渡せば、山田くんが荷物を持っていることになります。

```
  ┌──────────┐
  │    30    │ ← 名前: a
  └──────────┘ ← 名前: r （同じ箱に付いた2つ目の名前）
```

`&a`（アドレス）や `&r` を表示してみると、同じアドレスが出てきます。

### ポインタと比べてみる

Cのポインタでも同じことはできました。並べて比べてみます。

```cpp
// ポインタ版（Cと同じ書き方）
int a = 10;
int* p = &a;    // a のアドレスを p にしまう
*p = 20;        // 使うたびに * が必要

// 参照版
int b = 10;
int& r = b;     // b の別名を作る（& も * も右辺に要らない）
r = 20;         // 普通の変数と同じように使える
```

| | ポインタ `int* p` | 参照 `int& r` |
|---|---|---|
| 作るとき | `p = &a;`（アドレスを取る） | `int& r = a;`（そのまま書く） |
| 使うとき | `*p = 20;`（`*` が必要） | `r = 20;`（普通の変数と同じ） |
| 初期化しないで作る | できてしまう（中身は不定） | **コンパイルエラー** |
| NULL を入れる | できてしまう | **できない** |
| 途中で別の変数を指す | できる（`p = &b;`） | **できない**（一生同じ変数の別名） |

参照は「ポインタからできることを減らして、その分事故を起こせなくしたもの」と考えてください。
**できることが少ない＝間違えようがない**、というのが参照の安全さの正体です。

> **注意: 参照は付け替えられない**
>
> ```cpp
> int a = 1, b = 2;
> int& r = a;
> r = b;      // 「r を b の別名にする」ではない！
>             // 「a に b の値(2)を代入する」という意味になる
> ```
>
> 参照は作った瞬間に相手が決まり、以後ずっとその変数の別名です。

### 例: 2つの変数を入れ替える swap

Cの授業で一度は書いたであろう `swap` 関数を、参照で書き直してみます。

```cpp
#include <iostream>

// C流: void SwapByPointer(int* first, int* second) {
//          int saved = *first; *first = *second; *second = saved;
//      }
//      呼び出し: SwapByPointer(&left_count, &right_count);
void SwapValues(int& first, int& second) {
    int saved_first = first;   // 上書きされる前に first の値を退避
    first = second;
    second = saved_first;
}

int main() {
    int left_count = 1;    // 左ラインの部品数
    int right_count = 2;   // 右ラインの部品数
    SwapValues(left_count, right_count);                   // & を付け忘れる心配がない
    std::cout << left_count << " " << right_count << "\n"; // 2 1
    return 0;
}
```

関数の中に `*` が1つも出てこないので、普通の変数を扱うのと同じ感覚で読み書きできます。
また、ポインタ版では `SwapByPointer(NULL, &right_count)` のような呼び出しもコンパイルが通ってしまいますが、
参照版では「実在する変数」しか渡せないので、そもそもそういう間違いが書けません。

（実は標準ライブラリに `std::swap` が用意されているので、実際にはそれを使えば十分です。）

### 記号 `&` の2つの意味に注意

C++ では `&` が場所によって違う意味になるので、最初は混乱しがちです。

```cpp
int& r = a;     // 型名のすぐ後ろの &  → 「参照型」（宣言のとき）
int* p = &a;    // 変数の前の &        → 「アドレスを取る」（Cと同じ）
```

見分け方は「**型の一部として書かれているか（宣言）**、**式の中で変数に付いているか**」です。
`int&` のように型にくっついていれば参照、と覚えてください。

### 2.4 の範囲for との関係

2.4 で出てきた `for (double& temperature : temperatures)` の `&` も、この参照です。

- `for (double temperature : temperatures)` … `temperature` は要素の**コピー**。
  書き換えても `temperatures` は変わらない
- `for (double& temperature : temperatures)` … `temperature` は要素の**別名**。
  書き換えると `temperatures` の中身が変わる

「書き換えたいなら `&`」というルールは、関数の引数でも範囲forでも同じです。

### 参照を使うときの目安

この章の段階では、次の使い方だけ覚えておけば十分です。

1. **関数に呼び出し元の変数を書き換えてもらいたい** → 引数を `型&` にする
2. **範囲forで要素を書き換えたい** → `for (型& x : コンテナ)` にする

さらに「大きなデータを書き換えずに、コピーせず渡す」ための **const参照** という重要な使い方があります。
これは第3章（3.3）で詳しく扱います。

## 2.7 const を積極的に使う

書き換えない変数・引数には `const` を付けます。うっかり代入がコンパイルエラーになり、
コードを読む人への「これは変わらない」という約束にもなります。

```cpp
const double kStandardGravity = 9.80665;   // #define GRAVITY 9.80665 の代わり
kStandardGravity = 10.0;                   // コンパイルエラー！
```

Cの `#define` による定数はただの文字列置換で型もスコープもありませんでした。
C++ では定数は `const`（またはコンパイル時定数なら `constexpr`）で書きます。
定数の名前は先頭に `k` を付けた `kStandardGravity` の形にします（次の 2.8 で説明します）。

ただし `k` を付けるのは、`kStandardGravity` のように**プログラムを書いた時点で値が決まっている定数**だけです。
関数の中で計算した結果を「この後は書き換えない」という意味で `const` にした変数
（例: `const double mean = sum / count;`）は、普通の変数と同じ snake_case で書きます。

## 2.8 名前の付け方 — 読める名前は最高のコメント

プログラムは書く時間より**読まれる時間の方がずっと長い**ものです（数か月後の自分も「他人」です）。
`t`, `hi`, `lo`, `data`, `tmp` のような名前は、書いた瞬間は意味が分かっていても、
後から読むと「何の t？」「何の最大？」が分かりません。

```cpp
// 悪い例: 何を計算しているのか、コードを最後まで読まないと分からない
double s = 0.0, hi = v[0];
for (double x : v) { s += x; if (x > hi) hi = x; }

// 良い例: 名前を読むだけで「温度の合計と最高温度」だと分かる
double temperature_sum = 0.0;
double max_temperature = temperatures[0];
for (double temperature : temperatures) {
    temperature_sum += temperature;
    if (temperature > max_temperature) max_temperature = temperature;
}
```

名前を付けるときのコツ:

- **「何の値か」を具体的に書く**: `data` → `temperatures`、`f` → `resultant_force`、`n` → `num_intervals`
- **コンテナは複数形、要素は単数形**: `for (double stress : stresses)`
- **単位が紛らわしいときは名前かコメントに書く**: `area_mm2`、`double load_newton`、`// [MPa]`
- **略語は避ける**（`temp` は温度にも一時変数にも見える）。ただし `i`（ループの添字）、
  `x`（数式 f(x) の x）のように誰でも分かる慣習は、短い範囲でなら使ってよい

### 命名規則（このチュートリアルの約束）

名前の**書き方**（大文字・小文字・`_` の使い方）は、チームでルールを1つに決めて揃えるのが鉄則です。
このチュートリアルでは、世界中で広く参照されている
[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html#Naming) の命名規則に従います。

| 種類 | 書き方 | 例 |
|------|--------|----|
| 変数・関数の引数 | 小文字 + `_` 区切り（snake_case） | `max_temperature`, `time_step` |
| 定数（`constexpr` / プログラム全体を通して値が固定の `const`） | `k` + 単語の頭を大文字 | `kPi`, `kStandardGravity` |
| 関数 | 単語の頭を大文字（PascalCase） | `Increment()`, `MeanAndStddev()` |
| 型（struct / class / enum） | 単語の頭を大文字（PascalCase） | `Vec2`, `SensorSample` |
| クラスの private メンバ変数 | snake_case + 末尾に `_` | `mass_`, `area_mm2_`（第4章） |
| 名前空間 | snake_case | `numlib`, `sim`（第8章） |
| マクロ | すべて大文字 + `_` | `DATA_DIR`（第5章） |

> **標準ライブラリの名前と見た目が違うのは？**
> `push_back` や `size` のように、標準ライブラリ（`std::`）の関数は snake_case です。
> 標準ライブラリには標準ライブラリの流儀があり、**自分たちのコードは自分たちの規則で揃える**、
> というのが普通の考え方です。`std::` が付いているかどうかで、自作か標準かが一目で分かるという利点もあります。

## 2.9 まとめ: 置き換え表

| Cの道具 | C++の道具 | 章 |
|---------|-----------|----|
| `printf` / `scanf` | `std::cout` / `std::cin` | 2 |
| `char[]` + string.h | `std::string` | 2 |
| 固定長配列 / malloc | `std::vector` | 2 |
| `for (i = 0; ...)` | 範囲for | 2 |
| ポインタ渡し | 参照渡し | 2, 3 |
| `#define` 定数 | `const` / `constexpr` | 2 |
| ポインタ + malloc/free | スマートポインタ | 6 |

## 2.10 演習

`work\ch02` に新しいプロジェクトを作って（CMakeLists.txt は第1章のコピーでOK）、
次のプログラムを書いてください。変数には 2.8 の規則で**中身が分かる名前**を付けましょう。

1. **温度統計:** `std::vector<double>` に適当な温度データを10個入れ、
   範囲forを使って「平均値」「最大値」「最小値」を計算して表示する。
   （max/min は最初の要素で初期化してから範囲forで更新すればよい）
2. **名前の逆順表示:** `std::vector<std::string>` に名前を5つ入れ、
   逆順に表示する（ヒント: 添字を使う従来のforで書く。`std::size_t` は 0 未満にならない型なので、
   `i = size() - 1` から始めて `i >= 0` まで減らす書き方は**無限ループ**になります。
   `i` を 1 から `size()` まで増やし、`names[names.size() - i]` を表示する、のように工夫しましょう）
3. （発展）ユーザーが `end` と入力するまで `std::getline` で文字列を読み続けて
   vector に貯め、最後に全件と件数を表示する。

解答例: [`examples/ch02_from_c/solutions/`](../examples/ch02_from_c/solutions/)

次章: [第3章 関数の進化](03_functions.md)
