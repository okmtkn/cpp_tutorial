// 第7章 演習1: 線形補間テンプレート
// Vec2 で動かすには operator+ / operator- / スカラー倍(double * T)が必要
#include <cmath>
#include <iostream>

// ratio = 0 で start、ratio = 1 で end を返す
template <typename T>
T Lerp(T start, T end, double ratio) {
    return start + ratio * (end - start);
}

struct Vec2 {
    double x = 0.0;
    double y = 0.0;
};

Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}
Vec2 operator*(double scale, const Vec2& vec) {
    return {scale * vec.x, scale * vec.y};
}

std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
    return os << "(" << vec.x << ", " << vec.y << ")";
}

int main() {
    // double での補間
    std::cout << Lerp(10.0, 20.0, 0.25) << "\n";  // 12.5

    // Vec2 での補間（ロボットアーム先端の軌道生成などの基本）
    Vec2 start_point{0.0, 0.0};
    Vec2 goal_point{10.0, 5.0};
    // ratio を 0.25 ずつ足すのではなく、整数で回して計算する（第3章 3.4 の注意）
    constexpr int kNumDivisions = 4;
    for (int i = 0; i <= kNumDivisions; ++i) {
        const double ratio = static_cast<double>(i) / kNumDivisions;
        std::cout << "ratio = " << ratio << ": "
                  << Lerp(start_point, goal_point, ratio) << "\n";
    }
    return 0;
}
