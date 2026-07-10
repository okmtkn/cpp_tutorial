# 第4章 クラス — データと処理をまとめる

Cの `struct` は「データの入れ物」でした。C++ の**クラス**は、データに加えて
**そのデータを扱う関数（メンバ関数）** をまとめられます。
この章では2次元ベクトルクラスを題材に、クラスの基本一式を学びます。

サンプルコード: [`examples/ch04_classes/`](../examples/ch04_classes/)

## 4.1 struct にメンバ関数を持たせる

まずCの struct との地続きから始めます。C++ の struct は関数を持てます。

```cpp
#include <cmath>
#include <iostream>

struct Vec2 {
    double x;
    double y;

    // メンバ関数: この struct のデータ (x, y) を使う関数
    double norm() const {
        return std::sqrt(x * x + y * y);
    }
};

int main() {
    Vec2 f = {3.0, 4.0};                     // 力ベクトル [N]
    std::cout << f.norm() << "\n";           // 5 — 「f の norm を計算して」と読める
    return 0;
}
```

Cなら `vec2_norm(&f)` と書いていたところが `f.norm()` になります。
**データ（x, y）と処理（norm）が1つの型にまとまる**のがクラスの第一の利点です。

> `norm()` の後ろの `const` は「このメンバ関数はデータを書き換えません」という宣言です。
> 読むだけのメンバ関数には必ず付ける癖をつけてください。

## 4.2 コンストラクタ — 初期化を強制する

Cの struct は初期化を忘れてもコンパイルが通り、ゴミ値バグの温床でした。
**コンストラクタ**を定義すると、オブジェクト作成時に必ず初期化が走ります。

```cpp
struct Vec2 {
    double x;
    double y;

    // コンストラクタ: 型名と同じ名前、戻り値なし
    Vec2(double x_, double y_) : x(x_), y(y_) {}

    // 引数なし版（デフォルトコンストラクタ）: ゼロベクトル
    Vec2() : x(0.0), y(0.0) {}

    double norm() const { return std::sqrt(x * x + y * y); }
};

int main() {
    Vec2 a(3.0, 4.0);   // コンストラクタ呼び出し
    Vec2 b;             // (0, 0) で初期化される
    Vec2 c{1.0, 2.0};   // {} でも呼べる（こちらが新しい書き方）
    ...
}
```

`: x(x_), y(y_)` は**メンバ初期化子リスト**という書き方で、
コンストラクタでのメンバ初期化はこの形で書くのが基本です。

## 4.3 private と public — 不正な状態を作らせない

クラスの本領は**データを隠して、正しい操作だけを公開する**ことです。
`class` キーワードを使うと、既定でメンバが `private`（外から触れない）になります。

```cpp
#include <iostream>
#include <stdexcept>

// 材料試験片: 断面積は正の値しか許さない
class Specimen {
public:
    Specimen(double area_mm2) : area_mm2_(area_mm2) {
        if (area_mm2 <= 0.0) {
            throw std::invalid_argument("断面積は正の値にしてください");
        }
    }

    // 応力 [MPa] = 荷重 [N] / 断面積 [mm^2]
    double stress(double load_N) const { return load_N / area_mm2_; }

    double area() const { return area_mm2_; }   // 読み取り用（getter）

private:
    double area_mm2_;   // 外から直接書き換えられない
};

int main() {
    Specimen sp(25.0);                        // 5mm x 5mm の試験片
    std::cout << sp.stress(5000.0) << " MPa\n";  // 200 MPa

    // sp.area_mm2_ = -1.0;   // コンパイルエラー！ 不正な状態を作れない
    return 0;
}
```

- メンバ変数は `private` にして、必要な操作だけ `public` のメンバ関数で公開する
- private メンバ変数の名前は末尾に `_` を付ける流儀が広く使われています
- `struct` と `class` の違いは**既定が public か private かだけ**です。
  「ただのデータの束は struct、不変条件（正の断面積など）を守りたいものは class」が使い分けの目安です。

## 4.4 演算子オーバーロード — 数式のように書く

ベクトルの足し算を `a + b` と書けるようにできます。数値計算で威力絶大です。

```cpp
#include <cmath>
#include <iostream>

struct Vec2 {
    double x = 0.0;   // メンバ変数はここで既定値を書いてもよい（C++11以降）
    double y = 0.0;

    double norm() const { return std::sqrt(x * x + y * y); }
};

// 演算子オーバーロード（クラスの外に書く流儀）
Vec2 operator+(const Vec2& a, const Vec2& b) { return {a.x + b.x, a.y + b.y}; }
Vec2 operator-(const Vec2& a, const Vec2& b) { return {a.x - b.x, a.y - b.y}; }
Vec2 operator*(double k, const Vec2& v)      { return {k * v.x, k * v.y}; }

// std::cout << v と書けるようにする（お約束の形）
std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

int main() {
    Vec2 f1{3.0, 0.0};    // 力1 [N]
    Vec2 f2{0.0, 4.0};    // 力2 [N]

    Vec2 resultant = f1 + f2;             // 合力
    std::cout << "合力: " << resultant
              << " 大きさ: " << resultant.norm() << " N\n";

    Vec2 doubled = 2.0 * f1;              // スカラー倍
    std::cout << doubled << "\n";
    return 0;
}
```

`operator+` は普通の関数です。コンパイラが `a + b` を `operator+(a, b)` に読み替えます。
これは `std::string` の `+` 連結や、`std::cout <<` がずっと使ってきた仕組みそのものです。

## 4.5 クラスを組み合わせる — 質点の運動

クラスは他のクラスをメンバに持てます。Vec2 を使って質点（Particle）を作ってみます。

```cpp
class Particle {
public:
    Particle(double mass, Vec2 pos, Vec2 vel)
        : mass_(mass), pos_(pos), vel_(vel) {}

    // 力 f を dt 秒間受けたときの状態更新（オイラー法）
    void update(const Vec2& f, double dt) {
        Vec2 acc = (1.0 / mass_) * f;      // a = F/m
        vel_ = vel_ + dt * acc;
        pos_ = pos_ + dt * vel_;
    }

    Vec2 position() const { return pos_; }

private:
    double mass_;
    Vec2 pos_;
    Vec2 vel_;
};

int main() {
    // 質量 1kg、原点から初速 (10, 10) m/s で投射、重力だけ受ける
    Particle p(1.0, Vec2{0, 0}, Vec2{10, 10});
    const Vec2 gravity{0.0, -9.80665};

    for (int i = 0; i < 20; ++i) {
        p.update(gravity, 0.1);
        std::cout << p.position() << "\n";
    }
    return 0;
}
```

物理の式（a = F/m, v += a·dt, x += v·dt）がほぼそのままコードになっていることに
注目してください。**モデルとコードの見た目が近い**ことがクラス設計のゴールです。

## 4.6 演習

1. **Vec2 の拡張:** サンプルの Vec2 に「内積を返す関数 `double dot(const Vec2& a, const Vec2& b)`」と
   「単位ベクトルを返すメンバ関数 `Vec2 normalized() const`」を追加してください。
2. **Vec3 クラス:** 3次元ベクトル `Vec3` を自作し、+、-、スカラー倍、ノルム、内積、
   `<<` 出力を実装してください。外積 `cross(a, b)` も作れれば完璧です
   （モーメント M = r × F の計算に使えます）。
3. （発展）4.5 の Particle に空気抵抗（速度に比例する力 −c·v）を加えて、
   抵抗ありなしで到達距離がどう変わるか比べてみましょう。

解答例: [`examples/ch04_classes/solutions/`](../examples/ch04_classes/solutions/)

次章: [第5章 STLとラムダ式](05_stl.md)
