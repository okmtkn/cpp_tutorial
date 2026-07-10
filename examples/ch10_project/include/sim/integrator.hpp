#pragma once

#include <vector>

#include "sim/model.hpp"

namespace sim {

// 数値積分法の種類
enum class Method {
    SemiImplicitEuler,  // v を先に更新してから x を更新する（安定な定番）
    RK4,                // 4次ルンゲクッタ法（高精度）
};

// 1ステップ進める
State semi_implicit_euler_step(const SpringMassDamper& model, const State& s,
                               double dt);
State rk4_step(const SpringMassDamper& model, const State& s, double dt);

// t = 0 から t_end まで刻み dt でシミュレートし、全時刻の状態を返す
std::vector<State> simulate(const SpringMassDamper& model, double x0,
                            double v0, double dt, double t_end, Method method);

}  // namespace sim
