#include "sim/model.hpp"

#include <cmath>
#include <stdexcept>

namespace sim {

SpringMassDamper::SpringMassDamper(double m, double c, double k)
    : m_(m), c_(c), k_(k) {
    // 不正なパラメータはコンストラクタで拒否する（第4章）
    if (m <= 0.0) throw std::invalid_argument("mass must be positive");
    if (k <= 0.0) throw std::invalid_argument("stiffness must be positive");
    if (c < 0.0) throw std::invalid_argument("damping must be non-negative");
}

double SpringMassDamper::accel(double x, double v) const {
    return -(c_ * v + k_ * x) / m_;
}

double SpringMassDamper::omega() const { return std::sqrt(k_ / m_); }

double SpringMassDamper::zeta() const { return c_ / (2.0 * std::sqrt(m_ * k_)); }

double SpringMassDamper::exact_x(double t, double x0, double v0) const {
    const double w = omega();
    const double z = zeta();
    if (z >= 1.0) {
        throw std::domain_error("exact_x supports underdamped (zeta < 1) only");
    }
    const double wd = w * std::sqrt(1.0 - z * z);  // 減衰固有角振動数
    return std::exp(-z * w * t) *
           (x0 * std::cos(wd * t) + (v0 + z * w * x0) / wd * std::sin(wd * t));
}

}  // namespace sim
