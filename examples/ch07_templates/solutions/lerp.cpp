// 第7章 演習1: 線形補間テンプレート
// Vec2 で動かすには operator+ / operator- / スカラー倍(double * T)が必要
#include <cmath>
#include <iostream>

template <typename T>
T lerp(T a, T b, double t) {
    return a + t * (b - a);
}

struct Vec2 {
    double x = 0.0;
    double y = 0.0;
};

Vec2 operator+(const Vec2& a, const Vec2& b) { return {a.x + b.x, a.y + b.y}; }
Vec2 operator-(const Vec2& a, const Vec2& b) { return {a.x - b.x, a.y - b.y}; }
Vec2 operator*(double k, const Vec2& v) { return {k * v.x, k * v.y}; }

std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

int main() {
    // double での補間
    std::cout << lerp(10.0, 20.0, 0.25) << "\n";  // 12.5

    // Vec2 での補間（アニメーションや軌道生成の基本）
    Vec2 start{0.0, 0.0};
    Vec2 goal{10.0, 5.0};
    for (double t = 0.0; t <= 1.0; t += 0.25) {
        std::cout << "t = " << t << ": " << lerp(start, goal, t) << "\n";
    }
    return 0;
}
