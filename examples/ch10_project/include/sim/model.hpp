#pragma once

namespace sim {

// シミュレーションの状態: 時刻・変位・速度
struct State {
    double time = 0.0;          // [s]
    double displacement = 0.0;  // 変位 x [m]
    double velocity = 0.0;      // 速度 v [m/s]
};

// 1自由度振動系: m*x'' + c*x' + k*x = 0
class SpringMassDamper {
public:
    // mass: 質量 m [kg], damping: 減衰係数 c [N*s/m], stiffness: ばね定数 k [N/m]
    SpringMassDamper(double mass, double damping, double stiffness);

    // 加速度 a = -(c*v + k*x) / m
    double Acceleration(double displacement, double velocity) const;

    double NaturalFrequency() const;  // 固有角振動数 ω = sqrt(k/m) [rad/s]
    double DampingRatio() const;      // 減衰比 ζ = c / (2*sqrt(m*k)) [-]

    // 不足減衰 (ζ < 1) のときの理論解 x(t)。テストでの検証に使う。
    double ExactDisplacement(double time, double initial_displacement,
                             double initial_velocity) const;

private:
    double mass_;
    double damping_;
    double stiffness_;
};

}  // namespace sim
