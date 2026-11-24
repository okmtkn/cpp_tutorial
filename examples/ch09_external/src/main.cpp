// fmt を使った表出力のデモ
#include <fmt/core.h>

#include <string>
#include <utility>
#include <vector>

#include "mathutil.hpp"

int main() {
    std::string selected_material = "aluminum";
    double selected_modulus = 70.3;  // ヤング率 [GPa]

    // {} に順番に埋め込まれる。printf のような型指定は不要
    fmt::print("material: {}, E = {} GPa\n", selected_material,
               selected_modulus);

    // 書式指定の例: {:>8.1f} → 右寄せ・幅8文字・小数点以下1桁
    std::vector<std::pair<std::string, double>> young_modulus_table = {
        {"steel", 206.0},
        {"aluminum", 70.3},
        {"titanium", 106.0},
    };
    fmt::print("{:-^30}\n", " Young's modulus ");
    for (const auto& [material, young_modulus] : young_modulus_table) {
        fmt::print("{:<12} {:>8.1f} GPa\n", material, young_modulus);
    }

    // mathutil との組み合わせ
    std::vector<double> accelerations = {0.12, 0.34, 1.52, 2.87, 1.05};  // [m/s^2]
    fmt::print("\naccel mean = {:.3f}, rms = {:.3f} m/s^2\n",
               Mean(accelerations), RootMeanSquare(accelerations));
    return 0;
}
