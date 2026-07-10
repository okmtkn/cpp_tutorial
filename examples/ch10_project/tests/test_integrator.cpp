// 積分器のテスト: 理論解・エネルギー保存・不正パラメータの拒否
// 実行方法:  ctest --test-dir build --output-on-failure
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "sim/integrator.hpp"
#include "sim/model.hpp"

bool near(double a, double b, double tol) { return std::abs(a - b) < tol; }

int main() {
    const double x0 = 0.05, v0 = 0.0;

    // (1) RK4 は理論解とよく一致するはず
    {
        sim::SpringMassDamper model(1.0, 0.5, 100.0);
        auto hist = sim::simulate(model, x0, v0, 0.001, 2.0, sim::Method::RK4);
        const double exact = model.exact_x(2.0, x0, v0);
        assert(near(hist.back().x, exact, 1e-6));
    }

    // (2) セミインプリシット・オイラー法もそれなりに一致するはず（緩い許容誤差）
    {
        sim::SpringMassDamper model(1.0, 0.5, 100.0);
        auto hist = sim::simulate(model, x0, v0, 0.0001, 2.0,
                                  sim::Method::SemiImplicitEuler);
        const double exact = model.exact_x(2.0, x0, v0);
        assert(near(hist.back().x, exact, 1e-3));
    }

    // (3) 減衰なし (c=0) ならエネルギー E = (1/2)kx^2 + (1/2)mv^2 がほぼ保存する
    {
        const double m = 1.0, k = 100.0;
        sim::SpringMassDamper model(m, 0.0, k);
        auto hist = sim::simulate(model, x0, v0, 0.0001, 5.0,
                                  sim::Method::SemiImplicitEuler);
        const double E0 = 0.5 * k * x0 * x0 + 0.5 * m * v0 * v0;
        const auto& last = hist.back();
        const double E1 = 0.5 * k * last.x * last.x + 0.5 * m * last.v * last.v;
        assert(near(E1, E0, 0.01 * E0));  // 1% 以内
    }

    // (4) 不正なパラメータはコンストラクタが拒否するはず
    {
        bool thrown = false;
        try {
            sim::SpringMassDamper bad(-1.0, 0.5, 100.0);
        } catch (const std::invalid_argument&) {
            thrown = true;
        }
        assert(thrown);
    }

    std::cout << "all tests passed\n";
    return 0;
}
