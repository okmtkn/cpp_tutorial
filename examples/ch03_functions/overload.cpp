// 3.1 オーバーロード / 3.2 デフォルト引数 / 3.6 constexpr
#include <iostream>

int square(int x) { return x * x; }
double square(double x) { return x * x; }

// 等加速度運動の変位 x = v0*t + (1/2)*a*t^2（a を省略すると重力加速度）
double displacement(double v0, double t, double a = 9.80665) {
    return v0 * t + 0.5 * a * t * t;
}

// constexpr: 引数が定数ならコンパイル時に計算される
constexpr double deg_to_rad(double deg) {
    return deg * 3.14159265358979323846 / 180.0;
}

int main() {
    std::cout << square(3) << "\n";    // int 版 → 9
    std::cout << square(1.5) << "\n";  // double 版 → 2.25

    std::cout << displacement(0.0, 2.0) << " m (earth)\n";
    std::cout << displacement(0.0, 2.0, 1.62) << " m (moon)\n";

    constexpr double angle = deg_to_rad(90.0);  // コンパイル時に計算済み
    std::cout << "90 deg = " << angle << " rad\n";
    return 0;
}
