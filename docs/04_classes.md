# 第4章 クラス — データと処理をまとめる

この章のテーマは一言でいうと **「データと、その正しい扱い方を1つにまとめる」** です。
まずはCの `struct` で困っていたことから振り返ります。

サンプルコード: [`examples/ch04_classes/`](../examples/ch04_classes/)

### まず、Cで何が困っていたか

Cの `struct` は「データの入れ物」でした。たとえば引張試験の試験片を表すなら、こう書きます。

```c
/* C言語: 試験片と、その応力を計算する関数 */
struct Specimen {
    double area_mm2;   /* 断面積 [mm^2] */
};

/* 応力 [MPa] = 荷重 [N] / 断面積 [mm^2] */
double SpecimenStress(const struct Specimen* specimen, double load_newton) {
    return load_newton / specimen->area_mm2;
}

int main(void) {
    struct Specimen steel_specimen;
    steel_specimen.area_mm2 = -5.0;   /* ★ 負の断面積！ でもコンパイルは通る */
    double stress = SpecimenStress(&steel_specimen, 5000.0);  /* おかしな値になる */
    /* ... */
    return 0;
}
```

この書き方には、困りごとが2つあります。

- **誰でも中身を書き換えられる**: 断面積に負の値や 0 を入れても、コンパイラは何も言いません。
  0 なら割り算で計算が破綻します。「正しい値しか入らない」ことを、プログラマが毎回気をつけるしかありません。
- **データと関数がばらばら**: `SpecimenStress` が `Specimen` 用の関数だということは、名前から推測するしかありません。
  関連する関数が増えると、どこに何があるか分からなくなります。

### C++の答え: 「取扱説明書つきの部品」にする

機械部品を思い浮かべてください。良い部品は、決められた使い方（ボルト穴、取付面）しかできないように作られていて、
内部の機構に勝手に手を突っ込めないようカバーが付いています。

C++の**クラス**も同じです。

- データと、それを扱う関数（**メンバ関数**）を1つの型にまとめる
- 中身にはカバーをかけ（**private**）、決められた操作だけを外に見せる（**public**）
- 作った瞬間に正しい値が入っているよう、初期化の手順を決めておく（**コンストラクタ**）

この章では2次元ベクトル `Vec2` などを題材に、次の順で学びます。

1. **4.1** struct に関数を持たせる（データと処理をまとめる）
2. **4.2** コンストラクタ（初期化を必ず行わせる）
3. **4.3** private と public（上の試験片の例を、不正な値が入らないように書き直します）
4. **4.4** 演算子オーバーロード（`horizontal_force + vertical_force` のように数式で書く）
5. **4.5** クラスを組み合わせる（質点の運動）

## 4.1 struct にメンバ関数を持たせる

まずCの struct との地続きから始めます。C++ の struct は関数を持てます。

```cpp
#include <cmath>
#include <iostream>

struct Vec2 {
    double x;
    double y;

    // メンバ関数: この struct のデータ (x, y) を使う関数
    double Norm() const {
        return std::sqrt(x * x + y * y);
    }
};

int main() {
    Vec2 force = {3.0, 4.0};                 // 力ベクトル [N]
    std::cout << force.Norm() << "\n";       // 5 — 「force の大きさを計算して」と読める
    return 0;
}
```

Cなら `vec2_norm(&force)` と書いていたところが `force.Norm()` になります。
**データ（x, y）と処理（Norm）が1つの型にまとまる**のがクラスの第一の利点です。

> `Norm()` の後ろの `const` は「このメンバ関数はデータを書き換えません」という宣言です。
> 読むだけのメンバ関数には必ず付ける癖をつけてください。

## 4.2 コンストラクタ — 初期化を強制する

Cの struct は初期化を忘れてもコンパイルが通り、ゴミ値バグの温床でした。
**コンストラクタ**を定義すると、オブジェクト作成時に必ず初期化が走ります。

```cpp
struct Vec2 {
    double x;
    double y;

    // コンストラクタ: 型名と同じ名前、戻り値なし
    Vec2(double initial_x, double initial_y) : x(initial_x), y(initial_y) {}

    // 引数なし版（デフォルトコンストラクタ）: ゼロベクトル
    Vec2() : x(0.0), y(0.0) {}

    double Norm() const { return std::sqrt(x * x + y * y); }
};

int main() {
    Vec2 applied_force(3.0, 4.0);   // コンストラクタ呼び出し
    Vec2 origin;                    // (0, 0) で初期化される
    Vec2 velocity{1.0, 2.0};        // {} でも呼べる（こちらが新しい書き方）
    ...
}
```

`: x(initial_x), y(initial_y)` は**メンバ初期化子リスト**という書き方で、
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
    explicit Specimen(double area_mm2) : area_mm2_(area_mm2) {
        if (area_mm2 <= 0.0) {
            // 不正な値なら「例外」を投げて、オブジェクトの作成そのものを中止する
            throw std::invalid_argument("area must be positive");
        }
    }

    // 応力 [MPa] = 荷重 [N] / 断面積 [mm^2]
    double Stress(double load_newton) const { return load_newton / area_mm2_; }

    double area_mm2() const { return area_mm2_; }   // 読み取り用（getter）

private:
    double area_mm2_;   // 外から直接書き換えられない
};

int main() {
    Specimen steel_specimen(25.0);                        // 5mm x 5mm の試験片
    std::cout << steel_specimen.Stress(5000.0) << " MPa\n";  // 200 MPa

    // steel_specimen.area_mm2_ = -1.0;   // コンパイルエラー！ 不正な状態を作れない

    try {
        Specimen invalid_specimen(-3.0);   // 不正な値はコンストラクタが拒否する
    } catch (const std::exception& error) {
        std::cout << "rejected: " << error.what() << "\n";   // rejected: area must be positive
    }
    return 0;
}
```

> **`throw` / `try` / `catch` — 例外のごく簡単な紹介**
> コンストラクタには戻り値がないので、Cのように「エラーなら -1 を返す」ことができません。
> そこで `throw` で**例外**を投げて「この値では作れない」と知らせます。
>
> - `throw` が実行されると、その場で処理が打ち切られ、呼び出し元へさかのぼって
>   `try { }` の後ろの `catch` を探しに行きます。`error.what()` で `throw` 時のメッセージを取り出せます。
> - **どこにも `catch` が無いと、プログラムはその場で異常終了**します。
>
> 例外については第7章（7.5）でもう少し詳しく扱います。

> **`explicit` とは？**
> 引数1つのコンストラクタに `explicit` を付けないと、`Specimen steel_specimen = 25.0;` のように
> double から Specimen への**暗黙の変換**が許されてしまいます。うっかりした変換を防ぐため、
> 引数1つのコンストラクタには `explicit` を付けるのが定番です。

- メンバ変数は `private` にして、必要な操作だけ `public` のメンバ関数で公開する
- 名前に単位を入れておく（`area_mm2`, `load_newton`）と、mm と m の取り違えのような
  単位ミスを防げます
- 命名規則（2.8）のとおり、**クラスの private メンバ変数は末尾に `_`** を付けます（`area_mm2_`）。
  引数やローカル変数と区別でき、「これはオブジェクトが持っているデータだ」と一目で分かります。
  一方、データの束としての struct のメンバ（`Vec2` の `x`, `y`）には `_` を付けません
- 関数名は `Stress()` のように PascalCase ですが、**メンバ変数をそのまま返すだけの関数（getter）**は
  例外的に変数と同じ名前（`area_mm2()`）にしてよい、というのが Google C++ Style Guide の決まりです
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

    double Norm() const { return std::sqrt(x * x + y * y); }
};

// 演算子オーバーロード（クラスの外に書く流儀）
// lhs / rhs は「左辺 / 右辺 (left / right hand side)」の意味で、演算子の引数の定番の名前
Vec2 operator+(const Vec2& lhs, const Vec2& rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
Vec2 operator-(const Vec2& lhs, const Vec2& rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }
Vec2 operator*(double scale, const Vec2& vec)    { return {scale * vec.x, scale * vec.y}; }

// std::cout << vec と書けるようにする（お約束の形。os は output stream の略で定番の名前）
std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
    return os << "(" << vec.x << ", " << vec.y << ")";
}

int main() {
    Vec2 horizontal_force{3.0, 0.0};    // 水平方向の力 [N]
    Vec2 vertical_force{0.0, 4.0};      // 鉛直方向の力 [N]

    Vec2 resultant_force = horizontal_force + vertical_force;   // 合力
    std::cout << "resultant: " << resultant_force
              << ", magnitude: " << resultant_force.Norm() << " N\n";

    Vec2 doubled_force = 2.0 * horizontal_force;                // スカラー倍
    std::cout << doubled_force << "\n";
    return 0;
}
```

`operator+` は普通の関数です。コンパイラが `lhs + rhs` を `operator+(lhs, rhs)` に読み替えます。
これは `std::string` の `+` 連結や、`std::cout <<` がずっと使ってきた仕組みそのものです。

> **4.2 の Vec2 と形が違うのはなぜ？**
> ここでは Vec2 にコンストラクタを書かず、メンバに既定値 `= 0.0` を書いています。
> コンストラクタを持たない「ただのデータの束」の struct は、Cの構造体と同じように
> `Vec2{3.0, 0.0}` や `return {x, y};` の形で中身を順に指定して作れ、
> 何も指定しなければ既定値の 0.0 になります。x, y にどんな値が入ってもよい Vec2 には、この簡単な形で十分です。

> **注意:** 定義したのは `double * Vec2` の形だけなので、`horizontal_force * 2.0`
> （Vec2 * double）と順番を逆に書くとコンパイルエラーになります。
> 逆順でも書きたければ `Vec2 operator*(const Vec2& vec, double scale)` も定義します。

## 4.5 クラスを組み合わせる — 質点の運動

クラスは他のクラスをメンバに持てます。Vec2 を使って質点（Particle）を作ってみます。

```cpp
class Particle {
public:
    Particle(double mass, Vec2 position, Vec2 velocity)
        : mass_(mass), position_(position), velocity_(velocity) {}

    // 力 force を time_step 秒間受けたときの状態更新
    // （セミインプリシット・オイラー法: 先に v を更新し、新しい v で x を更新する）
    void Update(const Vec2& force, double time_step) {
        Vec2 acceleration = (1.0 / mass_) * force;      // a = F/m
        velocity_ = velocity_ + time_step * acceleration;
        position_ = position_ + time_step * velocity_;
    }

    Vec2 position() const { return position_; }   // getter はメンバ変数と同じ名前

private:
    double mass_;     // [kg]
    Vec2 position_;   // [m]
    Vec2 velocity_;   // [m/s]
};

int main() {
    // 質量 1kg のボールを原点から初速 (10, 10) m/s で投射、重力だけ受ける
    Particle ball(1.0, Vec2{0, 0}, Vec2{10, 10});
    const Vec2 gravity_force{0.0, -9.80665};   // 1kg に働く重力 [N]
    constexpr double kTimeStep = 0.1;          // [s]

    for (int step = 0; step < 20; ++step) {
        ball.Update(gravity_force, kTimeStep);
        std::cout << ball.position() << "\n";
    }
    return 0;
}
```

物理の式（a = F/m, v += a·dt, x += v·dt）がほぼそのままコードになっていることに
注目してください。

> 数値解析の授業で習う（陽的）オイラー法は「古い v で x を更新」しますが、
> ここでは「**更新後の** v で x を更新」しています。これは**セミインプリシット・オイラー法**と呼ばれる
> 変形版で、振動や軌道の計算でエネルギーの誤差がたまりにくい性質があります（第10章で詳しく扱います）。**モデルとコードの見た目が近い**ことがクラス設計のゴールです。
`pos`, `vel`, `acc` と略さずに `position_`, `velocity_`, `acceleration` と書くと、
物理量の名前がそのまま読めるので、式との対応がさらに分かりやすくなります。

## 4.6 演習

演習は `work\ch04` などに自分のプロジェクトを作って取り組んでください
（サンプルの `vec2.cpp` には演習1の解答が組み込み済みです）。

1. **Vec2 の拡張:** 4.4 の Vec2 に「内積を返す関数 `double Dot(const Vec2& lhs, const Vec2& rhs)`」と
   「単位ベクトルを返すメンバ関数 `Vec2 Normalized() const`」を追加してください。
   （ゼロベクトルを正規化しようとすると 0 で割ることになります。どう扱うべきかも考えてみましょう）
2. **Vec3 クラス:** 3次元ベクトル `Vec3` を自作し、+、-、スカラー倍、ノルム、内積、
   `<<` 出力を実装してください。外積 `Cross(lhs, rhs)` も作れれば完璧です
   （モーメント M = r × F の計算に使えます）。
3. （発展）4.5 の Particle に空気抵抗（速度に比例する力 −c·v）を加えて、
   抵抗ありなしで到達距離がどう変わるか比べてみましょう。

解答例: [`examples/ch04_classes/solutions/`](../examples/ch04_classes/solutions/)

次章: [第5章 STLとラムダ式](05_stl.md)
