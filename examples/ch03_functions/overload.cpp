// 3.1 オーバーロード / 3.2 デフォルト引数 / 3.6 constexpr
#include <iostream>

constexpr double kPi = 3.14159265358979323846;
constexpr double kStandardGravity = 9.80665;  // 標準重力加速度 [m/s^2]
constexpr double kMoonGravity = 1.62;         // 月面の重力加速度 [m/s^2]

int Square(int value) { return value * value; }
double Square(double value) { return value * value; }

// 等加速度運動の変位 x = v0*t + (1/2)*a*t^2（加速度を省略すると重力加速度）
double Displacement(double initial_velocity, double elapsed_time,
                    double acceleration = kStandardGravity) {
    return initial_velocity * elapsed_time +
           0.5 * acceleration * elapsed_time * elapsed_time;
}

// constexpr: 引数が定数ならコンパイル時に計算される
constexpr double DegToRad(double degrees) { return degrees * kPi / 180.0; }

int main() {
    std::cout << Square(3) << "\n";    // int 版 → 9
    std::cout << Square(1.5) << "\n";  // double 版 → 2.25

    std::cout << Displacement(0.0, 2.0) << " m (earth)\n";
    std::cout << Displacement(0.0, 2.0, kMoonGravity) << " m (moon)\n";

    constexpr double kRightAngleRad = DegToRad(90.0);  // コンパイル時に計算済み
    std::cout << "90 deg = " << kRightAngleRad << " rad\n";
    return 0;
}
