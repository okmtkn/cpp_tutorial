// fmt を使った表出力のデモ
#include <fmt/core.h>

#include <string>
#include <utility>
#include <vector>

#include "mathutil.hpp"

int main() {
    std::string name = "aluminum";
    double E = 70.3;

    // {} に順番に埋め込まれる。printf のような型指定は不要
    fmt::print("material: {}, E = {} GPa\n\n", name, E);

    // 書式指定: {:8.2f} → 幅8桁・小数点以下2桁
    std::vector<std::pair<std::string, double>> table = {
        {"steel", 206.0},
        {"aluminum", 70.3},
        {"titanium", 106.0},
    };
    fmt::print("{:-^30}\n", " Young's modulus ");
    for (const auto& [mat, e] : table) {
        fmt::print("{:<12} {:>8.1f} GPa\n", mat, e);
    }

    // mathutil との組み合わせ
    std::vector<double> accel = {0.12, 0.34, 1.52, 2.87, 1.05};
    fmt::print("\naccel mean = {:.3f}, rms = {:.3f} m/s^2\n",
               mean(accel), rms(accel));
    return 0;
}
