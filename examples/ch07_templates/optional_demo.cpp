// 7.4 std::optional — 「値がないかもしれない」を型で表す
#include <iostream>
#include <map>
#include <optional>
#include <string>

// 材料名からヤング率 [GPa] を検索。見つからないかもしれないので optional
std::optional<double> find_young_modulus(const std::string& name) {
    static const std::map<std::string, double> table{
        {"steel", 206.0},
        {"aluminum", 70.3},
        {"copper", 120.0},
    };
    auto it = table.find(name);
    if (it == table.end()) {
        return std::nullopt;  // 値なし
    }
    return it->second;
}

int main() {
    for (const std::string name : {"steel", "rubber"}) {
        std::optional<double> E = find_young_modulus(name);
        if (E.has_value()) {
            std::cout << name << ": " << *E << " GPa\n";
        } else {
            std::cout << name << ": not registered\n";
        }
    }

    // 見つからなければ既定値、が1行で書ける
    double E = find_young_modulus("rubber").value_or(0.01);
    std::cout << "E = " << E << " GPa\n";
    return 0;
}
