# 第6章 RAIIとスマートポインタ — new/delete からの卒業

この章のテーマは一言でいうと **「後片付けを忘れない仕組み」** です。
名前は難しそうですが、やりたいことは単純なので、まずはCで困っていたことから振り返ります。

サンプルコード: [`examples/ch06_smart_ptr/`](../examples/ch06_smart_ptr/)

### まず、Cで何が困っていたか

Cでは `malloc` で借りたメモリは `free` で、`fopen` で開いたファイルは `fclose` で、
**自分で返す**決まりでした。実習室の工具を借りたら、使い終わったら自分で工具棚に戻すのと同じです。

ところが「自分で返す」方式は、人間がうっかりすると簡単に壊れます。
次のCのコードには、よくある返し忘れが潜んでいます。

```c
/* C言語: ファイルからセンサ値を読み込んで平均を求める（一部省略） */
int ReadAverage(const char* path, double* average) {
    double* samples = malloc(1000 * sizeof(double));  /* ① メモリを借りる */
    FILE* input_file = fopen(path, "r");               /* ② ファイルを借りる */
    if (input_file == NULL) {
        return -1;      /* ★ ファイルが無いときここで抜けると、samples が返されない */
    }
    /* ... 読み込みと計算 ... */
    fclose(input_file); /* ② を返す */
    free(samples);      /* ① を返す */
    return 0;
}
```

途中の `return -1;` で関数を抜けると、最後の `free` までたどり着きません。
これが **メモリリーク（解放し忘れ）** です。ほかにも、

- 同じメモリを2回 `free` してしまう（**二重解放**）
- `free` した後のメモリをうっかり使う（**解放後の使用**）

といったバグがあり、どれもコンパイルは通ってしまい、実行時にたまに落ちる厄介なものです。

### C++ の new / delete も、実は同じ問題を抱えている

C++には `malloc`/`free` に相当する `new`/`delete` があります。
章タイトルの「new/delete」はこれのことです。

```cpp
// Motor は 6.3 で定義するモータのクラス（ここでは形だけ見てください）
Motor* motor = new Motor(1);   // malloc に相当: 確保して、コンストラクタも呼ぶ
motor->Drive();
delete motor;                  // free に相当: デストラクタを呼んで、解放する
```

書き方が少し便利になっただけで、**「自分で delete する」ことに変わりはありません**。
返し忘れ・二重解放・解放後の使用は、そのまま起こります。
（このチュートリアルでは `new`/`delete` は「見て読めれば十分」です。自分では書きません。）

### C++ の答え: 「返す係」をオブジェクトに任せる

工具の例でいえば、C++のやり方は
**「工具室の出口を通ると、持っている工具が自動で棚に戻るゲート」** を付けるようなものです。
人間が覚えておかなくても、部屋（スコープ = `{ }` の範囲）を出れば必ず返却されます。

これを実現する仕組みが次の2つで、この章で順に学びます。

1. **RAII（6.1）** … 「`{ }` を抜けるときに自動で呼ばれる関数（デストラクタ）」に後片付けを書いておく考え方
2. **スマートポインタ（6.3〜6.5）** … RAII を使って「自動で delete してくれる」ようにしたポインタ

この章を終えると、**`new`/`delete` を一度も書かずに**、ポインタが必要な場面も扱えるようになります。

## 6.1 RAII — 後始末はデストラクタに任せる

第4章で学んだ**コンストラクタ**は「オブジェクトが生まれるときに必ず呼ばれる関数」でした。
その反対が**デストラクタ**で、「オブジェクトが消えるときに必ず呼ばれる関数」です。
名前はクラス名の前に `~`（チルダ）を付けたもの（`~クラス名()`）で、引数も戻り値もありません。

ローカル変数のオブジェクトは、**宣言された `{ }` を抜けるとき**に消えます。
つまりデストラクタは、`{ }` の終わりで**自動的に**呼ばれます。
`return` で途中で抜けても、エラーで抜けても、必ず呼ばれます。

ここに後片付け（`free` や `fclose` に相当する処理）を書いておけば、返し忘れは起きなくなります。
この設計の考え方を **RAII (Resource Acquisition Is Initialization)** と呼びます。
直訳は「リソースの確保は初期化である」で分かりにくいのですが、意味は
「**借りるのはコンストラクタで、返すのはデストラクタで**」と覚えれば十分です。
ここでいう**リソース**とは、メモリやファイルなど「借りたら返さなければならないもの」全般のことです。

まずは、コンストラクタとデストラクタがいつ呼ばれるかを目で確かめてみましょう。

```cpp
#include <iostream>

// 生成と破棄のタイミングを画面に表示するだけのクラス
class LifetimeLogger {
public:
    LifetimeLogger(const char* label) : label_(label) {
        std::cout << label_ << ": constructed\n";
    }
    ~LifetimeLogger() {   // デストラクタ
        std::cout << label_ << ": destructed\n";
    }
private:
    const char* label_;
};

int main() {
    LifetimeLogger outer_logger("outer");
    {
        LifetimeLogger inner_logger("inner");
        std::cout << "--- inner scope end ---\n";
    }                 // ← ここで inner_logger のデストラクタが自動で呼ばれる
    std::cout << "--- main end ---\n";
    return 0;
}                     // ← ここで outer_logger のデストラクタが自動で呼ばれる
```

実行結果:

```
outer: constructed
inner: constructed
--- inner scope end ---
inner: destructed
--- main end ---
outer: destructed
```

第5章の `std::ofstream` が close 不要だったのはこの仕組みです。
デストラクタでファイルを閉じているので、**関数の途中で return してもエラーで抜けても**
確実に閉じられます。`fclose` 忘れが原理的に起きません。

vector も string も同じで、内部でメモリを確保していますが、
スコープを抜ければデストラクタが解放してくれます。
**実は、ここまでの章で一度もメモリ解放を書いていないのに、リークは一度も起きていません。**

## 6.2 それでもポインタが必要になる場面

vector と string で大半は済みますが、次の場面ではポインタが必要になります。

1. **多態性（ポリモーフィズム）**: 「図形」の配列に円と長方形を混ぜて入れたい（6.4）
2. **大きなオブジェクトの所有権の移動**: コピーせずに受け渡したい
3. C言語のライブラリとの連携

そこで使うのが、**自動で delete してくれるポインタ = スマートポインタ**です。

## 6.3 std::unique_ptr — 唯一の所有者

`std::unique_ptr<T>` は「T へのポインタ。ただし**所有者は自分ひとり**で、
自分が消えるときに指す先も delete する」という型です。

```cpp
#include <iostream>
#include <memory>   // unique_ptr, make_unique

class Motor {
public:
    Motor(int id) : id_(id) { std::cout << "Motor " << id_ << " ON\n"; }
    ~Motor()                { std::cout << "Motor " << id_ << " OFF\n"; }
    void Drive() const      { std::cout << "Motor " << id_ << " driving\n"; }
private:
    int id_;
};

int main() {
    // new の代わりに make_unique を使う
    std::unique_ptr<Motor> conveyor_motor = std::make_unique<Motor>(1);

    conveyor_motor->Drive();   // 矢印演算子はCのポインタと同じ

    // delete は書かない。スコープを抜けると自動で解放される
    return 0;
}
```

### 所有権の移動 (move)

unique_ptr はコピーできません（所有者が2人になってしまうから）。
所有権を渡したいときは `std::move` で**移動**します。
倉庫の在庫モータを装置に取り付ける場面をイメージしてください。

```cpp
std::unique_ptr<Motor> stock_motor = std::make_unique<Motor>(1);
// std::unique_ptr<Motor> installed_motor = stock_motor;          // コンパイルエラー！ コピー禁止
std::unique_ptr<Motor> installed_motor = std::move(stock_motor);  // OK: 所有権が移る
// 以後 stock_motor は空（nullptr）。使ってはいけない
```

## 6.4 多態性 — スマートポインタの出番

「円も長方形も入る図形リスト」を作るには、**基底クラスのポインタ**経由で
オブジェクトを扱う必要があります。これがポインタが本質的に必要になる場面です。

```cpp
#include <iostream>
#include <memory>
#include <vector>

constexpr double kPi = 3.14159265358979323846;

// 基底クラス（インターフェース）
class Shape {
public:
    virtual ~Shape() = default;          // 基底クラスのデストラクタは virtual に（お約束）
    virtual double Area() const = 0;     // 純粋仮想関数: 派生クラスが必ず実装する
    virtual const char* Name() const = 0;
};

class Circle : public Shape {
public:
    Circle(double radius) : radius_(radius) {}
    double Area() const override { return kPi * radius_ * radius_; }
    const char* Name() const override { return "Circle"; }
private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}
    double Area() const override { return width_ * height_; }
    const char* Name() const override { return "Rectangle"; }
private:
    double width_;
    double height_;
};

int main() {
    // 「Shape の unique_ptr」の vector なら、違う型を混ぜて入れられる
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rectangle>(2.0, 3.0));
    shapes.push_back(std::make_unique<Circle>(0.5));

    double total_area = 0.0;
    for (const auto& shape : shapes) {
        std::cout << shape->Name() << ": " << shape->Area() << "\n";
        total_area += shape->Area();
    }
    std::cout << "total area: " << total_area << "\n";
    return 0;
}   // vector が消えるとき、各 unique_ptr が中身を自動で delete する
```

このコードには新しい文法がたくさん出てくるので、1つずつ見ていきます。

**継承 `class Circle : public Shape`**
「Circle は Shape の一種である」という宣言です（**継承**と呼びます）。
Shape を**基底クラス**（親）、Circle を**派生クラス**（子）といいます。
派生クラスは基底クラスのメンバ関数を受け継ぎ、`Shape` が必要な場所に `Circle` を渡せるようになります。
機械部品でいえば「ボルトもナットも『締結部品』の一種」という関係です。
（`public` は「Shape として公開されている操作は Circle でも公開する」という意味で、通常はこれを使います）

**仮想関数 `virtual` と `override`**
- `virtual` … 「基底クラスのポインタ経由で呼んでも、**指している実際の型**のメンバ関数を呼ぶ」ための指定。
  `shape->Area()` は、shape が Circle を指していれば Circle の `Area()` が、Rectangle なら Rectangle の `Area()` が呼ばれます。
- `= 0` … **純粋仮想関数**。「Shape 自身は中身を持たず、派生クラスが必ず実装する」という意味です。
  純粋仮想関数を持つクラスは、`Shape plain_shape;` のように直接オブジェクトを作れません。
- `override` … 「基底クラスの仮想関数を上書きしている」ことの明示。
  名前や引数を書き間違えるとコンパイルエラーにしてくれます（スペルミス検出）。

**なぜポインタが必要なのか**
`std::vector<Shape>` では、要素はすべて「Shape ちょうどの大きさの箱」になるため、
半径を持つ Circle や幅と高さを持つ Rectangle は入れられません（そもそも Shape は直接作れません）。
そこで「Shape を指すポインタ」の vector にして、実体はそれぞれの型で別に作ります。
ポインタの大きさはどの型を指しても同じなので、違う型を混ぜて並べられるのです。

**`virtual ~Shape() = default;`（仮想デストラクタ）**
unique_ptr は最後に「Shape へのポインタ」として delete します。デストラクタが virtual でないと、
Circle を指していても **Shape のデストラクタしか呼ばれず、後片付けが正しく行われません**（未定義動作）。
「基底クラスとして使うクラスのデストラクタは virtual」と覚えてください。
`= default` は「中身はコンパイラが自動で作る標準のものでよい」という意味です。

CAD の形状データ、有限要素法の要素タイプなど、
「共通の操作を持つ異なる種類のもの」の管理はこのパターンで書かれます。

## 6.5 std::shared_ptr — 共有する所有者

`shared_ptr` は所有者を複数持てるスマートポインタです。
内部で参照カウントを持ち、**最後の所有者が消えたときに**解放されます。

```cpp
#include <memory>
#include <iostream>

int main() {
    // 生産数のデータを、生産管理と監視モニタの2か所で共有する例
    std::shared_ptr<int> production_count = std::make_shared<int>(42);
    std::cout << production_count.use_count() << "\n";       // 1
    {
        std::shared_ptr<int> monitor_view = production_count; // コピーOK。所有者が2人に
        std::cout << production_count.use_count() << "\n";   // 2
    }                                     // monitor_view 消滅 → 所有者1人に戻る
    std::cout << production_count.use_count() << "\n";       // 1
    return 0;
}                                         // production_count 消滅 → 解放
```

**まずは unique_ptr、共有が本当に必要なときだけ shared_ptr** が使い分けの原則です。
shared_ptr は便利に見えますが、「誰がいつまで持っているのか」が分かりにくくなる
コストがあります。

## 6.6 現代C++のメモリ管理まとめ

| 状況 | 使うもの |
|------|---------|
| 値の列を持ちたい | `std::vector`（ポインタ不要） |
| 文字列を持ちたい | `std::string`（ポインタ不要） |
| 関数にデータを渡したい | 値 / `&` / `const &`（ポインタ不要） |
| 多態性・所有権の移動 | `std::unique_ptr` + `std::make_unique` |
| 所有権を複数で共有 | `std::shared_ptr` + `std::make_shared` |
| 生の `new` / `delete` | **書かない** |

## 6.7 演習

1. 6.4 の Shape の例に、三角形クラス `Triangle` を追加してください。
   簡単のため**直角三角形**とし、直角をはさむ2辺（底辺と高さ）で表します
   （演習3で周長を計算するとき、斜辺は三平方の定理で求められます）。
2. 図形リストから「面積が最大の図形」を探して名前と面積を表示してください
   （第5章の `std::max_element` + ラムダ式が使えます）。
3. （発展）`Shape` に `virtual double Perimeter() const`（周長）を追加し、
   全クラスに実装してください。円周率は `constexpr` で定義しましょう。

解答例: [`examples/ch06_smart_ptr/solutions/`](../examples/ch06_smart_ptr/solutions/)

次章: [第7章 テンプレート入門と std::optional](07_templates_optional.md)
