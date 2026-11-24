// 第4章 演習3 (発展): 空気抵抗（速度に比例する力 -c*v）ありの投射運動
// 抵抗ありなしで到達距離（y が 0 に戻るまでの x）を比較する
#include <cmath>
#include <iostream>

struct Vec2 {
    double x = 0.0;
    double y = 0.0;
};

Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
Vec2 operator*(double scale, const Vec2& vec) {
    return {scale * vec.x, scale * vec.y};
}

class Particle {
public:
    Particle(double mass, Vec2 position, Vec2 velocity)
        : mass_(mass), position_(position), velocity_(velocity) {}

    // セミインプリシット・オイラー法（先に v を更新し、新しい v で x を更新）
    void Update(const Vec2& force, double time_step) {
        Vec2 acceleration = (1.0 / mass_) * force;
        velocity_ = velocity_ + time_step * acceleration;
        position_ = position_ + time_step * velocity_;
    }

    Vec2 position() const { return position_; }
    Vec2 velocity() const { return velocity_; }

private:
    double mass_;
    Vec2 position_;
    Vec2 velocity_;
};

// 着地するまで飛ばして到達距離 [m] を返す
// drag_coefficient: 空気抵抗の係数 c [N*s/m]
double SimulateRange(double drag_coefficient) {
    Particle ball(1.0, Vec2{0, 0}, Vec2{10, 10});
    const Vec2 gravity_force{0.0, -9.80665};
    constexpr double kTimeStep = 0.001;

    while (ball.position().y >= 0.0) {
        // 合力 = 重力 + 空気抵抗 (-c * v)
        Vec2 total_force = gravity_force + (-drag_coefficient) * ball.velocity();
        ball.Update(total_force, kTimeStep);
    }
    return ball.position().x;
}

int main() {
    std::cout << "no drag  : " << SimulateRange(0.0) << " m\n";
    std::cout << "c = 0.1  : " << SimulateRange(0.1) << " m\n";
    std::cout << "c = 0.5  : " << SimulateRange(0.5) << " m\n";
    return 0;
}
