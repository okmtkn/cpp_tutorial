#include "sim/model.hpp"

#include <cmath>
#include <stdexcept>

namespace sim {

SpringMassDamper::SpringMassDamper(double mass, double damping,
                                   double stiffness)
    : mass_(mass), damping_(damping), stiffness_(stiffness) {
    // 不正なパラメータはコンストラクタで拒否する（第4章）
    if (mass <= 0.0) throw std::invalid_argument("mass must be positive");
    if (stiffness <= 0.0) throw std::invalid_argument("stiffness must be positive");
    if (damping < 0.0) throw std::invalid_argument("damping must be non-negative");
}

double SpringMassDamper::Acceleration(double displacement,
                                      double velocity) const {
    return -(damping_ * velocity + stiffness_ * displacement) / mass_;
}

double SpringMassDamper::NaturalFrequency() const {
    return std::sqrt(stiffness_ / mass_);
}

double SpringMassDamper::DampingRatio() const {
    return damping_ / (2.0 * std::sqrt(mass_ * stiffness_));
}

double SpringMassDamper::ExactDisplacement(double time,
                                           double initial_displacement,
                                           double initial_velocity) const {
    const double natural_frequency = NaturalFrequency();  // ω
    const double damping_ratio = DampingRatio();          // ζ
    if (damping_ratio >= 1.0) {
        throw std::domain_error(
            "ExactDisplacement supports underdamped (zeta < 1) only");
    }
    // 減衰固有角振動数 ω_d = ω * sqrt(1 - ζ^2)
    const double damped_frequency =
        natural_frequency * std::sqrt(1.0 - damping_ratio * damping_ratio);

    // x(t) = e^(-ζωt) * (x0*cos(ω_d t) + (v0 + ζω x0)/ω_d * sin(ω_d t))
    const double decay = std::exp(-damping_ratio * natural_frequency * time);
    const double cos_amplitude = initial_displacement;
    const double sin_amplitude =
        (initial_velocity +
         damping_ratio * natural_frequency * initial_displacement) /
        damped_frequency;
    return decay * (cos_amplitude * std::cos(damped_frequency * time) +
                    sin_amplitude * std::sin(damped_frequency * time));
}

}  // namespace sim
