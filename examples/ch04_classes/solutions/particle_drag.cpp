// 第4章 演習3 (発展): 空気抵抗（速度に比例する力 -c*v）ありの投射運動
// 抵抗ありなしで到達距離（y が 0 に戻るまでの x）を比較する
#include <cmath>
#include <iostream>

struct Vec2 {
    double x = 0.0;
    double y = 0.0;
};

Vec2 operator+(const Vec2& a, const Vec2& b) { return {a.x + b.x, a.y + b.y}; }
Vec2 operator*(double k, const Vec2& v) { return {k * v.x, k * v.y}; }

class Particle {
public:
    Particle(double mass, Vec2 pos, Vec2 vel)
        : mass_(mass), pos_(pos), vel_(vel) {}

    void update(const Vec2& f, double dt) {
        Vec2 acc = (1.0 / mass_) * f;
        vel_ = vel_ + dt * acc;
        pos_ = pos_ + dt * vel_;
    }

    Vec2 position() const { return pos_; }
    Vec2 velocity() const { return vel_; }

private:
    double mass_;
    Vec2 pos_;
    Vec2 vel_;
};

// 着地するまで飛ばして到達距離を返す
double simulate_range(double drag_c) {
    Particle p(1.0, Vec2{0, 0}, Vec2{10, 10});
    const Vec2 gravity{0.0, -9.80665};
    const double dt = 0.001;

    while (p.position().y >= 0.0) {
        // 合力 = 重力 + 空気抵抗 (-c * v)
        Vec2 f = gravity + (-drag_c) * p.velocity();
        p.update(f, dt);
    }
    return p.position().x;
}

int main() {
    std::cout << "no drag  : " << simulate_range(0.0) << " m\n";
    std::cout << "c = 0.1  : " << simulate_range(0.1) << " m\n";
    std::cout << "c = 0.5  : " << simulate_range(0.5) << " m\n";
    return 0;
}
