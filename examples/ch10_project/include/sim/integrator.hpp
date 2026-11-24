#pragma once

#include <vector>

#include "sim/model.hpp"

namespace sim {

// 数値積分法の種類（列挙子は k + PascalCase で名付ける）
enum class IntegrationMethod {
    kSemiImplicitEuler,  // v を先に更新してから x を更新する（安定な定番）
    kRk4,                // 4次ルンゲクッタ法（高精度）
};

// 1ステップ進める
State SemiImplicitEulerStep(const SpringMassDamper& model, const State& current,
                            double time_step);
State Rk4Step(const SpringMassDamper& model, const State& current,
              double time_step);

// 時刻 0 から end_time まで刻み time_step でシミュレートし、全時刻の状態を返す
std::vector<State> Simulate(const SpringMassDamper& model,
                            double initial_displacement,
                            double initial_velocity, double time_step,
                            double end_time, IntegrationMethod method);

}  // namespace sim
