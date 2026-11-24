// 積分器のテスト: 理論解・エネルギー保存・不正パラメータの拒否
// 実行方法:  ctest --test-dir build --output-on-failure
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "sim/integrator.hpp"
#include "sim/model.hpp"

bool IsNear(double actual, double expected, double tolerance) {
    return std::abs(actual - expected) < tolerance;
}

int main() {
    constexpr double kInitialDisplacement = 0.05;  // [m]
    constexpr double kInitialVelocity = 0.0;       // [m/s]

    // (1) RK4 は理論解とよく一致するはず
    {
        sim::SpringMassDamper oscillator(1.0, 0.5, 100.0);
        auto history =
            sim::Simulate(oscillator, kInitialDisplacement, kInitialVelocity,
                          0.001, 2.0, sim::IntegrationMethod::kRk4);
        const double exact_displacement = oscillator.ExactDisplacement(
            2.0, kInitialDisplacement, kInitialVelocity);
        assert(IsNear(history.back().displacement, exact_displacement, 1e-6));
    }

    // (2) セミインプリシット・オイラー法もそれなりに一致するはず（緩い許容誤差）
    {
        sim::SpringMassDamper oscillator(1.0, 0.5, 100.0);
        auto history = sim::Simulate(
            oscillator, kInitialDisplacement, kInitialVelocity, 0.0001, 2.0,
            sim::IntegrationMethod::kSemiImplicitEuler);
        const double exact_displacement = oscillator.ExactDisplacement(
            2.0, kInitialDisplacement, kInitialVelocity);
        assert(IsNear(history.back().displacement, exact_displacement, 1e-3));
    }

    // (3) 減衰なし (c=0) ならエネルギー E = (1/2)kx^2 + (1/2)mv^2 がほぼ保存する
    {
        constexpr double kMass = 1.0;
        constexpr double kStiffness = 100.0;
        sim::SpringMassDamper undamped_oscillator(kMass, 0.0, kStiffness);
        auto history = sim::Simulate(
            undamped_oscillator, kInitialDisplacement, kInitialVelocity, 0.0001,
            5.0, sim::IntegrationMethod::kSemiImplicitEuler);

        const double initial_energy =
            0.5 * kStiffness * kInitialDisplacement * kInitialDisplacement +
            0.5 * kMass * kInitialVelocity * kInitialVelocity;
        const sim::State& final_state = history.back();
        const double final_energy =
            0.5 * kStiffness * final_state.displacement * final_state.displacement +
            0.5 * kMass * final_state.velocity * final_state.velocity;
        assert(IsNear(final_energy, initial_energy, 0.01 * initial_energy));  // 1% 以内
    }

    // (4) 不正なパラメータはコンストラクタが拒否するはず
    {
        bool was_rejected = false;
        try {
            sim::SpringMassDamper negative_mass_oscillator(-1.0, 0.5, 100.0);
        } catch (const std::invalid_argument&) {
            was_rejected = true;
        }
        assert(was_rejected);
    }

    std::cout << "all tests passed\n";
    return 0;
}
