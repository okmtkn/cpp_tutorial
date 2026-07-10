#pragma once

namespace sim {

// シミュレーションの状態: 時刻・変位・速度
struct State {
    double t = 0.0;  // [s]
    double x = 0.0;  // [m]
    double v = 0.0;  // [m/s]
};

// 1自由度振動系: m*x'' + c*x' + k*x = 0
class SpringMassDamper {
public:
    // m: 質量 [kg], c: 減衰係数 [N*s/m], k: ばね定数 [N/m]
    SpringMassDamper(double m, double c, double k);

    // 加速度 a = -(c*v + k*x) / m
    double accel(double x, double v) const;

    double omega() const;  // 固有角振動数 ω = sqrt(k/m) [rad/s]
    double zeta() const;   // 減衰比 ζ = c / (2*sqrt(m*k)) [-]

    // 不足減衰 (ζ < 1) のときの理論解 x(t)。テストでの検証に使う。
    double exact_x(double t, double x0, double v0) const;

private:
    double m_;
    double c_;
    double k_;
};

}  // namespace sim
