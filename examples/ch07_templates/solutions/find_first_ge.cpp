// 第7章 演習2: x 以上になる最初の要素の添字を optional で返す
#include <iostream>
#include <optional>
#include <vector>

std::optional<std::size_t> find_first_ge(const std::vector<double>& v,
                                         double x) {
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (v[i] >= x) return i;
    }
    return std::nullopt;
}

int main() {
    std::vector<double> load = {0.0, 12.5, 30.2, 55.8, 80.1};  // 荷重 [kN]

    for (double threshold : {50.0, 100.0}) {
        auto idx = find_first_ge(load, threshold);
        if (idx) {
            std::cout << threshold << " kN reached at index " << *idx << "\n";
        } else {
            std::cout << threshold << " kN never reached\n";
        }
    }
    return 0;
}
