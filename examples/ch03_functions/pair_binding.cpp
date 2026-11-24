// 3.5 複数の値を返す — std::pair と構造化束縛
#include <iostream>
#include <utility>
#include <vector>

// 最小値と最大値を同時に返す（values は空であってはならない）
std::pair<double, double> MinMax(const std::vector<double>& values) {
    double min_value = values[0];
    double max_value = values[0];
    for (double value : values) {
        if (value < min_value) min_value = value;
        if (value > max_value) max_value = value;
    }
    return {min_value, max_value};
}

int main() {
    std::vector<double> stresses = {120.5, 98.2, 141.7, 110.3};  // 応力 [MPa]

    // 構造化束縛で2変数に受け取る
    auto [min_stress, max_stress] = MinMax(stresses);
    std::cout << "min: " << min_stress << " MPa, max: " << max_stress
              << " MPa\n";
    return 0;
}
