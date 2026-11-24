// 4.1〜4.4 2次元ベクトルクラス — メンバ関数・演算子オーバーロード
// （演習1の Dot / Normalized も実装済み）
#include <cmath>
#include <iostream>

struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    // const: このメンバ関数はデータを書き換えない、という宣言
    double Norm() const { return std::sqrt(x * x + y * y); }

    // 演習1: 単位ベクトル（ゼロベクトルに使うと 0 除算になるので注意）
    Vec2 Normalized() const {
        const double length = Norm();
        return {x / length, y / length};
    }
};

// 演算子オーバーロード: lhs + rhs を operator+(lhs, rhs) と読み替えてくれる
// （lhs / rhs は「左辺 / 右辺 (left/right hand side)」の意味）
Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}
Vec2 operator*(double scale, const Vec2& vec) {
    return {scale * vec.x, scale * vec.y};
}

// 演習1: 内積
double Dot(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

// std::cout << vec と書けるようにする（お約束の形）
std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
    return os << "(" << vec.x << ", " << vec.y << ")";
}

int main() {
    Vec2 horizontal_force{3.0, 0.0};  // 水平方向の力 [N]
    Vec2 vertical_force{0.0, 4.0};    // 鉛直方向の力 [N]

    Vec2 resultant_force = horizontal_force + vertical_force;  // 合力
    std::cout << "resultant: " << resultant_force
              << ", magnitude: " << resultant_force.Norm() << " N\n";

    std::cout << "2 * horizontal = " << 2.0 * horizontal_force << "\n";
    std::cout << "dot = " << Dot(horizontal_force, vertical_force)
              << "\n";  // 直交 → 0
    std::cout << "unit vector: " << resultant_force.Normalized() << "\n";
    return 0;
}
