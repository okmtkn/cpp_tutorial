// 4.1〜4.4 2次元ベクトルクラス — メンバ関数・演算子オーバーロード
// （演習1の dot / normalized も実装済み）
#include <cmath>
#include <iostream>

struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    // const: このメンバ関数はデータを書き換えない、という宣言
    double norm() const { return std::sqrt(x * x + y * y); }

    // 演習1: 単位ベクトル
    Vec2 normalized() const {
        const double n = norm();
        return {x / n, y / n};
    }
};

// 演算子オーバーロード: a + b を operator+(a, b) と読み替えてくれる
Vec2 operator+(const Vec2& a, const Vec2& b) { return {a.x + b.x, a.y + b.y}; }
Vec2 operator-(const Vec2& a, const Vec2& b) { return {a.x - b.x, a.y - b.y}; }
Vec2 operator*(double k, const Vec2& v) { return {k * v.x, k * v.y}; }

// 演習1: 内積
double dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }

// std::cout << v と書けるようにする（お約束の形）
std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

int main() {
    Vec2 f1{3.0, 0.0};  // 力1 [N]
    Vec2 f2{0.0, 4.0};  // 力2 [N]

    Vec2 resultant = f1 + f2;  // 合力
    std::cout << "resultant: " << resultant
              << ", magnitude: " << resultant.norm() << " N\n";

    std::cout << "2 * f1 = " << 2.0 * f1 << "\n";
    std::cout << "dot(f1, f2) = " << dot(f1, f2) << "\n";       // 直交 → 0
    std::cout << "unit vector: " << resultant.normalized() << "\n";
    return 0;
}
