// ばね・質量・ダンパ系シミュレータ本体
// 実行すると result.csv に時間履歴を出力する（Excel や Python でグラフ化して確認）
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

#include "sim/integrator.hpp"
#include "sim/model.hpp"

int main() {
    // ---- パラメータ ----
    const double m = 1.0;    // 質量 [kg]
    const double c = 0.5;    // 減衰係数 [N*s/m]
    const double k = 100.0;  // ばね定数 [N/m]
    const double x0 = 0.05;  // 初期変位 [m]
    const double v0 = 0.0;   // 初期速度 [m/s]
    const double dt = 0.001; // 時間刻み [s]
    const double t_end = 5.0;

    sim::SpringMassDamper model(m, c, k);
    std::cout << "omega = " << model.omega() << " rad/s, "
              << "zeta = " << model.zeta() << "\n";

    // ---- シミュレーション（RK4）----
    auto history = sim::simulate(model, x0, v0, dt, t_end, sim::Method::RK4);

    // ---- CSV出力 ----
    std::ofstream out("result.csv");
    out << "t,x,v\n";
    for (const sim::State& s : history) {
        out << s.t << "," << s.x << "," << s.v << "\n";
    }
    std::cout << "wrote result.csv (" << history.size() << " rows)\n";

    // ---- 最大振幅（減衰していれば初期変位 x0 のはず）----
    auto peak = std::max_element(
        history.begin(), history.end(),
        [](const sim::State& a, const sim::State& b) {
            return std::abs(a.x) < std::abs(b.x);
        });
    std::cout << "max |x| = " << std::abs(peak->x) << " m at t = " << peak->t
              << " s\n";

    // ---- 精度比較: オイラー法 vs RK4 vs 理論解（t_end 時点）----
    auto euler = sim::simulate(model, x0, v0, dt, t_end,
                               sim::Method::SemiImplicitEuler);
    const double exact = model.exact_x(t_end, x0, v0);
    std::cout << "x(t_end): exact = " << exact
              << ", euler err = " << std::abs(euler.back().x - exact)
              << ", rk4 err = " << std::abs(history.back().x - exact) << "\n";
    return 0;
}
