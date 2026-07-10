// 4.5 クラスを組み合わせる — 質点の投射運動（オイラー法）
#include <cmath>
#include <iostream>

struct Vec2 {
    double x = 0.0;
    double y = 0.0;
    double norm() const { return std::sqrt(x * x + y * y); }
};

Vec2 operator+(const Vec2& a, const Vec2& b) { return {a.x + b.x, a.y + b.y}; }
Vec2 operator*(double k, const Vec2& v) { return {k * v.x, k * v.y}; }

std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

class Particle {
public:
    Particle(double mass, Vec2 pos, Vec2 vel)
        : mass_(mass), pos_(pos), vel_(vel) {}

    // 力 f を dt 秒間受けたときの状態更新（オイラー法）
    void update(const Vec2& f, double dt) {
        Vec2 acc = (1.0 / mass_) * f;  // a = F/m
        vel_ = vel_ + dt * acc;
        pos_ = pos_ + dt * vel_;
    }

    Vec2 position() const { return pos_; }

private:
    double mass_;
    Vec2 pos_;
    Vec2 vel_;
};

int main() {
    // 質量 1kg、初速 (10, 10) m/s で投射、重力だけを受ける
    Particle p(1.0, Vec2{0, 0}, Vec2{10, 10});
    const Vec2 gravity{0.0, -9.80665};

    for (int i = 0; i < 20; ++i) {
        p.update(gravity, 0.1);
        std::cout << p.position() << "\n";
    }
    return 0;
}
