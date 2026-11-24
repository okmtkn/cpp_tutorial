// 5.3 std::map — キーと値の対応表
#include <iostream>
#include <map>
#include <string>

int main() {
    // 材料名 → ヤング率 [GPa]
    std::map<std::string, double> young_modulus_by_material{
        {"steel", 206.0},
        {"aluminum", 70.3},
        {"copper", 120.0},
    };

    young_modulus_by_material["titanium"] = 106.0;  // 追加

    // 読むだけなら at()（キーが無ければ例外。勝手に登録されない）
    std::cout << "steel: " << young_modulus_by_material.at("steel") << " GPa\n";

    // 全要素（キーの昇順）。構造化束縛が活躍
    for (const auto& [material, young_modulus] : young_modulus_by_material) {
        std::cout << material << ": " << young_modulus << " GPa\n";
    }

    // 存在チェック。[] で読むと「存在しないキーを値0で勝手に登録」するので注意
    if (young_modulus_by_material.count("concrete") == 0) {
        std::cout << "concrete is not registered\n";
    }
    return 0;
}
