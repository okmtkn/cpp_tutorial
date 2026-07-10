#include "sim/integrator.hpp"

#include <cmath>

namespace sim {

State semi_implicit_euler_step(const SpringMassDamper& model, const State& s,
                               double dt) {
    State next;
    next.v = s.v + model.accel(s.x, s.v) * dt;  // v を先に更新
    next.x = s.x + next.v * dt;                 // 更新後の v で x を更新
    next.t = s.t + dt;
    return next;
}

State rk4_step(const SpringMassDamper& model, const State& s, double dt) {
    // 状態 (x, v) の微分は (v, a(x, v))
    const double k1x = s.v;
    const double k1v = model.accel(s.x, s.v);

    const double k2x = s.v + 0.5 * dt * k1v;
    const double k2v = model.accel(s.x + 0.5 * dt * k1x, s.v + 0.5 * dt * k1v);

    const double k3x = s.v + 0.5 * dt * k2v;
    const double k3v = model.accel(s.x + 0.5 * dt * k2x, s.v + 0.5 * dt * k2v);

    const double k4x = s.v + dt * k3v;
    const double k4v = model.accel(s.x + dt * k3x, s.v + dt * k3v);

    State next;
    next.x = s.x + dt / 6.0 * (k1x + 2.0 * k2x + 2.0 * k3x + k4x);
    next.v = s.v + dt / 6.0 * (k1v + 2.0 * k2v + 2.0 * k3v + k4v);
    next.t = s.t + dt;
    return next;
}

std::vector<State> simulate(const SpringMassDamper& model, double x0,
                            double v0, double dt, double t_end, Method method) {
    std::vector<State> history;
    State s{0.0, x0, v0};
    history.push_back(s);

    const int steps = static_cast<int>(std::round(t_end / dt));
    for (int i = 0; i < steps; ++i) {
        s = (method == Method::RK4) ? rk4_step(model, s, dt)
                                    : semi_implicit_euler_step(model, s, dt);
        history.push_back(s);
    }
    return history;
}

}  // namespace sim
