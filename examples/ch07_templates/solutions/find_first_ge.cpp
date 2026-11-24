// 第7章 演習2: threshold 以上になる最初の要素の添字を optional で返す
#include <iostream>
#include <optional>
#include <vector>

std::optional<std::size_t> FindFirstAtLeast(const std::vector<double>& values,
                                            double threshold) {
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (values[i] >= threshold) return i;
    }
    return std::nullopt;
}

int main() {
    std::vector<double> loads = {0.0, 12.5, 30.2, 55.8, 80.1};  // 荷重 [kN]

    for (double target_load : {50.0, 100.0}) {
        auto reached_index = FindFirstAtLeast(loads, target_load);
        if (reached_index) {
            std::cout << target_load << " kN reached at index " << *reached_index
                      << "\n";
        } else {
            std::cout << target_load << " kN never reached\n";
        }
    }
    return 0;
}
