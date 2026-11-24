// 7.4 std::optional — 「値がないかもしれない」を型で表す
#include <iostream>
#include <map>
#include <optional>
#include <string>

// 材料名からヤング率 [GPa] を検索。見つからないかもしれないので optional
std::optional<double> FindYoungModulus(const std::string& material) {
    // static な定数（プログラム終了まで残る）は k + PascalCase で名付ける
    static const std::map<std::string, double> kYoungModulusTable{
        {"steel", 206.0},
        {"aluminum", 70.3},
        {"copper", 120.0},
    };
    auto entry = kYoungModulusTable.find(material);
    if (entry == kYoungModulusTable.end()) {
        return std::nullopt;  // 値なし
    }
    return entry->second;
}

int main() {
    for (const std::string material : {"steel", "rubber"}) {
        std::optional<double> young_modulus = FindYoungModulus(material);
        if (young_modulus.has_value()) {
            std::cout << material << ": " << *young_modulus << " GPa\n";
        } else {
            std::cout << material << ": not registered\n";
        }
    }

    // 見つからなければ既定値、が1行で書ける
    double rubber_modulus = FindYoungModulus("rubber").value_or(0.01);
    std::cout << "rubber (default) = " << rubber_modulus << " GPa\n";
    return 0;
}
