#include "sim/integrator.hpp"

#include <cmath>

namespace sim {

State SemiImplicitEulerStep(const SpringMassDamper& model, const State& current,
                            double time_step) {
    State next;
    // v を先に更新
    next.velocity = current.velocity +
                    model.Acceleration(current.displacement, current.velocity) *
                        time_step;
    // 更新後の v で x を更新
    next.displacement = current.displacement + next.velocity * time_step;
    next.time = current.time + time_step;
    return next;
}

State Rk4Step(const SpringMassDamper& model, const State& current,
              double time_step) {
    // 状態 (x, v) の時間微分は (v, a(x, v))。
    // k1〜k4 は RK4 の教科書どおりの「傾き」の名前で、
    // *_displacement は変位の傾き（=速度）、*_velocity は速度の傾き（=加速度）。
    const double half_step = 0.5 * time_step;
    const double x = current.displacement;  // 式を読みやすくするための短い別名
    const double v = current.velocity;

    const double k1_displacement = v;
    const double k1_velocity = model.Acceleration(x, v);

    const double k2_displacement = v + half_step * k1_velocity;
    const double k2_velocity = model.Acceleration(
        x + half_step * k1_displacement, v + half_step * k1_velocity);

    const double k3_displacement = v + half_step * k2_velocity;
    const double k3_velocity = model.Acceleration(
        x + half_step * k2_displacement, v + half_step * k2_velocity);

    const double k4_displacement = v + time_step * k3_velocity;
    const double k4_velocity = model.Acceleration(
        x + time_step * k3_displacement, v + time_step * k3_velocity);

    State next;
    next.displacement =
        x + time_step / 6.0 *
                (k1_displacement + 2.0 * k2_displacement +
                 2.0 * k3_displacement + k4_displacement);
    next.velocity = v + time_step / 6.0 *
                            (k1_velocity + 2.0 * k2_velocity +
                             2.0 * k3_velocity + k4_velocity);
    next.time = current.time + time_step;
    return next;
}

std::vector<State> Simulate(const SpringMassDamper& model,
                            double initial_displacement,
                            double initial_velocity, double time_step,
                            double end_time, IntegrationMethod method) {
    std::vector<State> history;
    State state{0.0, initial_displacement, initial_velocity};
    history.push_back(state);

    const int num_steps = static_cast<int>(std::round(end_time / time_step));
    for (int step = 0; step < num_steps; ++step) {
        state = (method == IntegrationMethod::kRk4)
                    ? Rk4Step(model, state, time_step)
                    : SemiImplicitEulerStep(model, state, time_step);
        history.push_back(state);
    }
    return history;
}

}  // namespace sim
