// 第4章 演習2: 3次元ベクトルクラス（外積 = モーメント計算付き）
#include <cmath>
#include <iostream>

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    double norm() const { return std::sqrt(x * x + y * y + z * z); }
};

Vec3 operator+(const Vec3& a, const Vec3& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
Vec3 operator-(const Vec3& a, const Vec3& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
Vec3 operator*(double k, const Vec3& v) { return {k * v.x, k * v.y, k * v.z}; }

double dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// 外積: モーメント M = r × F の計算に使う
Vec3 cross(const Vec3& a, const Vec3& b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

int main() {
    // 位置 r [m] に力 F [N] が働くときの原点まわりモーメント
    Vec3 r{0.5, 0.0, 0.0};
    Vec3 F{0.0, 100.0, 0.0};

    Vec3 M = cross(r, F);
    std::cout << "M = r x F = " << M << " [N*m]\n";  // (0, 0, 50)
    std::cout << "|M| = " << M.norm() << " N*m\n";
    std::cout << "dot(r, F) = " << dot(r, F) << " (perpendicular)\n";
    return 0;
}
