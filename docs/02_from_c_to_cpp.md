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
    std::cout << "年齢を入力してください: ";
    std::cin >> age;
    std::cout << "あなたは " << age << " 歳です\n";
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
    std::string name = "Okamoto";
    std::string greeting = "Hello, " + name + "!";  // + で連結できる

    std::cout << greeting << "\n";
    std::cout << "長さ: " << greeting.size() << "\n";      // strlen 相当
    std::cout << "先頭文字: " << greeting[0] << "\n";

    if (name == "Okamoto") {   // strcmp ではなく == で比較できる
        std::cout << "名前が一致しました\n";
    }

    std::string line;
    std::cout << "1行入力してください: ";
    std::getline(std::cin, line);   // 空白を含む1行を読む（gets の安全版）
    std::cout << "入力: " << line << "\n";
    return 0;
}
```

| Cでの書き方 | C++での書き方 |
|-------------|---------------|
| `char s[64]; strcpy(s, "abc");` | `std::string s = "abc";` |
| `strcat(s, t)` | `s += t` または `s + t` |
| `strlen(s)` | `s.size()` |
| `strcmp(s, t) == 0` | `s == t` |

## 2.3 配列: 固定長配列 → std::vector

`std::vector` は**サイズを自動管理してくれる可変長配列**です。
`malloc`/`free`/`realloc` は不要になります。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {10, 20, 30};   // int の可変長配列

    v.push_back(40);       // 末尾に追加（自動でメモリが伸びる）
    v.push_back(50);

    std::cout << "要素数: " << v.size() << "\n";
    std::cout << "先頭: " << v[0] << ", 末尾: " << v.back() << "\n";

    // 従来の for でも回せる
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";
    return 0;
}
```

`std::vector<int>` の `<int>` は「int を入れる vector」という意味です。
`std::vector<double>` や `std::vector<std::string>` も同じように作れます
（この仕組み＝テンプレートは第7章で学びます）。

## 2.4 範囲for文 — 「全要素を順に処理」の決定版

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<double> temps = {23.5, 24.1, 22.8, 25.0};

    // 「temps の各要素 t について」と読む
    for (double t : temps) {
        std::cout << t << " ";
    }
    std::cout << "\n";

    // 要素を書き換えたいときは参照 & を付ける（2.6参照）
    for (double& t : temps) {
        t += 273.15;   // 摂氏 → 絶対温度
    }

    for (double t : temps) {
        std::cout << t << " ";
    }
    std::cout << "\n";
    return 0;
}
```

添字 `i` を書かないので、**off-by-one エラー（`<=` と `<` の取り違え等）が原理的に起きません**。
「全要素を先頭から順に」処理するときは常に範囲forを使いましょう。

## 2.5 型推論: auto

`auto` と書くと、コンパイラが右辺から型を推論してくれます。

```cpp
auto x = 42;          // int
auto y = 3.14;        // double
auto s = std::string("hello");
auto v = std::vector<int>{1, 2, 3};

for (auto t : temps) { ... }   // 範囲forでもよく使う
```

型名が長くなるC++では多用しますが、**「型がわからなくなるまで」使うのはNG**です。
右辺を見て型が明らかなときに使う、が目安です。

## 2.6 参照 — ポインタの安全な代替

Cで「関数で呼び出し元の変数を書き換える」にはポインタを使いました。
C++ では**参照（reference）**が使えます。参照は「変数の別名」です。

```cpp
#include <iostream>

// C流:  void increment(int* p) { (*p)++; }  → 呼び出しは increment(&a);
// C++流: 参照で受け取る
void increment(int& n) {
    n++;   // n は呼び出し元の変数そのもの
}

int main() {
    int a = 10;
    increment(a);            // & も * も書かない
    std::cout << a << "\n";  // 11
    return 0;
}
```

- 参照は**必ず初期化が必要**で、**NULLになれない**ため、ヌルポインタ事故が起きません。
- ポインタが不要になったわけではありませんが（第6章で再登場）、
  「関数に変数を書き換えてもらう」用途は参照で書くのがC++流です。

## 2.7 const を積極的に使う

書き換えない変数・引数には `const` を付けます。うっかり代入がコンパイルエラーになり、
コードを読む人への「これは変わらない」という約束にもなります。

```cpp
const double GRAVITY = 9.80665;   // #define GRAVITY 9.80665 の代わり
GRAVITY = 10.0;                   // コンパイルエラー！
```

Cの `#define` による定数はただの文字列置換で型もスコープもありませんでした。
C++ では定数は `const`（またはコンパイル時定数なら `constexpr`）で書きます。

## 2.8 まとめ: 置き換え表

| Cの道具 | C++の道具 | 章 |
|---------|-----------|----|
| `printf` / `scanf` | `std::cout` / `std::cin` | 2 |
| `char[]` + string.h | `std::string` | 2 |
| 固定長配列 / malloc | `std::vector` | 2 |
| `for (i = 0; ...)` | 範囲for | 2 |
| ポインタ渡し | 参照渡し | 2, 3 |
| `#define` 定数 | `const` / `constexpr` | 2 |
| ポインタ + malloc/free | スマートポインタ | 6 |

## 2.9 演習

`work\ch02` に新しいプロジェクトを作って（CMakeLists.txt は第1章のコピーでOK）、
次のプログラムを書いてください。

1. **温度統計:** `std::vector<double>` に適当な温度データを10個入れ、
   範囲forを使って「平均値」「最大値」「最小値」を計算して表示する。
   （max/min は最初の要素で初期化してから範囲forで更新すればよい）
2. **名前の逆順表示:** `std::vector<std::string>` に名前を5つ入れ、
   逆順に表示する（ヒント: 添字を使う従来のforで `i = v.size() - 1` から回す。
   `std::size_t` は 0 未満にならない型なので終了条件に注意！）
3. （発展）ユーザーが `end` と入力するまで `std::getline` で文字列を読み続けて
   vector に貯め、最後に全件と件数を表示する。

解答例: [`examples/ch02_from_c/solutions/`](../examples/ch02_from_c/solutions/)

次章: [第3章 関数の進化](03_functions.md)
