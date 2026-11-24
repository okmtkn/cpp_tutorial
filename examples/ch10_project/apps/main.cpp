// ばね・質量・ダンパ系シミュレータ本体
// 実行すると result.csv に時間履歴を出力する（Excel や Python でグラフ化して確認）
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

#include "sim/integrator.hpp"
#include "sim/model.hpp"

int main() {
    // ---- パラメータ（実行中に変わらない定数なので k + PascalCase）----
    constexpr double kMass = 1.0;                  // 質量 m [kg]
    constexpr double kDamping = 0.5;               // 減衰係数 c [N*s/m]
    constexpr double kStiffness = 100.0;           // ばね定数 k [N/m]
    constexpr double kInitialDisplacement = 0.05;  // 初期変位 x0 [m]
    constexpr double kInitialVelocity = 0.0;       // 初期速度 v0 [m/s]
    constexpr double kTimeStep = 0.001;            // 時間刻み dt [s]
    constexpr double kEndTime = 5.0;               // 終了時刻 [s]

    sim::SpringMassDamper oscillator(kMass, kDamping, kStiffness);
    std::cout << "omega = " << oscillator.NaturalFrequency() << " rad/s, "
              << "zeta = " << oscillator.DampingRatio() << "\n";

    // ---- シミュレーション（RK4）----
    auto rk4_history =
        sim::Simulate(oscillator, kInitialDisplacement, kInitialVelocity,
                      kTimeStep, kEndTime, sim::IntegrationMethod::kRk4);

    // ---- CSV出力 ----
    std::ofstream csv_file("result.csv");
    csv_file << "time,displacement,velocity\n";
    for (const sim::State& state : rk4_history) {
        csv_file << state.time << "," << state.displacement << ","
                 << state.velocity << "\n";
    }
    std::cout << "wrote result.csv (" << rk4_history.size() << " rows)\n";

    // ---- 最大振幅（減衰していれば初期変位のはず）----
    auto peak_state = std::max_element(
        rk4_history.begin(), rk4_history.end(),
        [](const sim::State& lhs, const sim::State& rhs) {
            return std::abs(lhs.displacement) < std::abs(rhs.displacement);
        });
    std::cout << "max |x| = " << std::abs(peak_state->displacement)
              << " m at t = " << peak_state->time << " s\n";

    // ---- 精度比較: オイラー法 vs RK4 vs 理論解（終了時刻の時点）----
    auto euler_history = sim::Simulate(
        oscillator, kInitialDisplacement, kInitialVelocity, kTimeStep, kEndTime,
        sim::IntegrationMethod::kSemiImplicitEuler);
    const double exact_displacement = oscillator.ExactDisplacement(
        kEndTime, kInitialDisplacement, kInitialVelocity);
    const double euler_error =
        std::abs(euler_history.back().displacement - exact_displacement);
    const double rk4_error =
        std::abs(rk4_history.back().displacement - exact_displacement);
    std::cout << "x(t_end): exact = " << exact_displacement
              << ", euler err = " << euler_error << ", rk4 err = " << rk4_error
              << "\n";
    return 0;
}
