// 4.5 クラスを組み合わせる — 質点の投射運動（セミインプリシット・オイラー法）
#include <cmath>
#include <iostream>

struct Vec2 {
    double x = 0.0;
    double y = 0.0;
    double Norm() const { return std::sqrt(x * x + y * y); }
};

Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
Vec2 operator*(double scale, const Vec2& vec) {
    return {scale * vec.x, scale * vec.y};
}

std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
    return os << "(" << vec.x << ", " << vec.y << ")";
}

class Particle {
public:
    Particle(double mass, Vec2 position, Vec2 velocity)
        : mass_(mass), position_(position), velocity_(velocity) {}

    // 力 force を time_step 秒間受けたときの状態更新
    // （セミインプリシット・オイラー法: 先に v を更新し、新しい v で x を更新する）
    void Update(const Vec2& force, double time_step) {
        Vec2 acceleration = (1.0 / mass_) * force;  // a = F/m
        velocity_ = velocity_ + time_step * acceleration;
        position_ = position_ + time_step * velocity_;
    }

    // メンバ変数をそのまま返すだけの関数（アクセサ）は、変数名と同じ名前にしてよい
    Vec2 position() const { return position_; }

private:
    double mass_;     // [kg]
    Vec2 position_;   // [m]
    Vec2 velocity_;   // [m/s]
};

int main() {
    // 質量 1kg のボールを初速 (10, 10) m/s で投射、重力だけを受ける
    Particle ball(1.0, Vec2{0, 0}, Vec2{10, 10});
    const Vec2 gravity_force{0.0, -9.80665};  // 1kg に働く重力 [N]
    constexpr double kTimeStep = 0.1;         // [s]

    for (int step = 0; step < 20; ++step) {
        ball.Update(gravity_force, kTimeStep);
        std::cout << ball.position() << "\n";
    }
    return 0;
}
