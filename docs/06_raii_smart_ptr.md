# 第6章 RAIIとスマートポインタ — new/delete からの卒業

Cの `malloc`/`free` は「解放し忘れ（メモリリーク）」「二重解放」「解放後の使用」という
三大バグの温床でした。モダンC++の答えは
**「メモリ管理をコンパイラに任せる（RAII + スマートポインタ）」** です。

この章を終えると、**`new`/`delete` を直接書かないC++** が書けるようになります。

サンプルコード: [`examples/ch06_smart_ptr/`](../examples/ch06_smart_ptr/)

## 6.1 RAII — 後始末はデストラクタに任せる

**RAII (Resource Acquisition Is Initialization)** は
「リソースの確保をコンストラクタで、解放をデストラクタで行う」という設計原則です。

デストラクタ（`~クラス名()`）は、**オブジェクトがスコープを抜けるとき自動で呼ばれる**
特別なメンバ関数です。

```cpp
#include <iostream>

class Probe {
public:
    Probe(const char* name) : name_(name) {
        std::cout << name_ << ": constructed\n";
    }
    ~Probe() {   // デストラクタ
        std::cout << name_ << ": destructed\n";
    }
private:
    const char* name_;
};

int main() {
    Probe a("a");
    {
        Probe b("b");
        std::cout << "--- inner scope end ---\n";
    }                 // ← ここで b のデストラクタが自動で呼ばれる
    std::cout << "--- main end ---\n";
    return 0;
}                     // ← ここで a のデストラクタが自動で呼ばれる
```

実行結果:

```
a: constructed
b: constructed
--- inner scope end ---
b: destructed
--- main end ---
a: destructed
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
    void drive() const      { std::cout << "Motor " << id_ << " driving\n"; }
private:
    int id_;
};

int main() {
    // new の代わりに make_unique を使う
    std::unique_ptr<Motor> m = std::make_unique<Motor>(1);

    m->drive();          // 矢印演算子はCのポインタと同じ

    // delete は書かない。スコープを抜けると自動で解放される
    return 0;
}
```

### 所有権の移動 (move)

unique_ptr はコピーできません（所有者が2人になってしまうから）。
所有権を渡したいときは `std::move` で**移動**します。

```cpp
std::unique_ptr<Motor> a = std::make_unique<Motor>(1);
// std::unique_ptr<Motor> b = a;             // コンパイルエラー！ コピー禁止
std::unique_ptr<Motor> b = std::move(a);     // OK: 所有権が a から b へ移る
// 以後 a は空（nullptr）。使ってはいけない
```

## 6.4 多態性 — スマートポインタの出番

「円も長方形も入る図形リスト」を作るには、**基底クラスのポインタ**経由で
オブジェクトを扱う必要があります。これがポインタが本質的に必要になる場面です。

```cpp
#include <iostream>
#include <memory>
#include <vector>

// 基底クラス（インターフェース）
class Shape {
public:
    virtual ~Shape() = default;          // 基底クラスのデストラクタは virtual に（お約束）
    virtual double area() const = 0;     // 純粋仮想関数: 派生クラスが必ず実装する
    virtual const char* name() const = 0;
};

class Circle : public Shape {
public:
    Circle(double r) : r_(r) {}
    double area() const override { return 3.14159265358979 * r_ * r_; }
    const char* name() const override { return "Circle"; }
private:
    double r_;
};

class Rect : public Shape {
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    const char* name() const override { return "Rect"; }
private:
    double w_, h_;
};

int main() {
    // 「Shape の unique_ptr」の vector なら、違う型を混ぜて入れられる
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rect>(2.0, 3.0));
    shapes.push_back(std::make_unique<Circle>(0.5));

    double total = 0.0;
    for (const auto& s : shapes) {
        std::cout << s->name() << ": " << s->area() << "\n";
        total += s->area();
    }
    std::cout << "合計面積: " << total << "\n";
    return 0;
}   // vector が消えるとき、各 unique_ptr が中身を自動で delete する
```

- `virtual` … 「ポインタの指す実際の型のメンバ関数を呼ぶ」ための指定
- `override` … 「基底クラスの仮想関数を上書きしている」ことの明示（スペルミス検出になる）
- CAD の形状データ、有限要素法の要素タイプなど、
  「共通の操作を持つ異なる種類のもの」の管理はこのパターンで書かれます。

## 6.5 std::shared_ptr — 共有する所有者

`shared_ptr` は所有者を複数持てるスマートポインタです。
内部で参照カウントを持ち、**最後の所有者が消えたときに**解放されます。

```cpp
#include <memory>
#include <iostream>

int main() {
    std::shared_ptr<int> a = std::make_shared<int>(42);
    std::cout << a.use_count() << "\n";   // 1
    {
        std::shared_ptr<int> b = a;       // コピーOK。所有者が2人に
        std::cout << a.use_count() << "\n";   // 2
    }                                     // b 消滅 → 所有者1人に戻る
    std::cout << a.use_count() << "\n";   // 1
    return 0;
}                                         // a 消滅 → 解放
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

1. 6.4 の Shape の例に、三角形クラス `Triangle`（底辺と高さ）を追加してください。
2. 図形リストから「面積が最大の図形」を探して名前と面積を表示してください
   （第5章の `std::max_element` + ラムダ式が使えます）。
3. （発展）`Shape` に `virtual double perimeter() const`（周長）を追加し、
   全クラスに実装してください。円周率は `constexpr` で定義しましょう。

解答例: [`examples/ch06_smart_ptr/solutions/`](../examples/ch06_smart_ptr/solutions/)

次章: [第7章 テンプレート入門と std::optional](07_templates_optional.md)
