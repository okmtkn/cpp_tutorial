// 第4章 演習2: 3次元ベクトルクラス（外積 = モーメント計算付き）
#include <cmath>
#include <iostream>

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    double Norm() const { return std::sqrt(x * x + y * y + z * z); }
};

Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}
Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}
Vec3 operator*(double scale, const Vec3& vec) {
    return {scale * vec.x, scale * vec.y, scale * vec.z};
}

double Dot(const Vec3& lhs, const Vec3& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

// 外積: モーメント M = r × F の計算に使う
Vec3 Cross(const Vec3& lhs, const Vec3& rhs) {
    return {lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x};
}

std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
    return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

int main() {
    // 原点から位置 r [m] の点に力 F [N] が働くときの原点まわりモーメント
    Vec3 lever_arm{0.5, 0.0, 0.0};  // 位置ベクトル r [m]
    Vec3 force{0.0, 100.0, 0.0};    // 力 F [N]

    Vec3 moment = Cross(lever_arm, force);
    std::cout << "M = r x F = " << moment << " [N*m]\n";  // (0, 0, 50)
    std::cout << "|M| = " << moment.Norm() << " N*m\n";
    std::cout << "dot(r, F) = " << Dot(lever_arm, force)
              << " (perpendicular)\n";
    return 0;
}
