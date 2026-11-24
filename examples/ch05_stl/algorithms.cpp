// 5.1 algorithm の定番: accumulate / max_element / min_element / sort
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<double> temperatures = {23.5, 21.2, 25.8, 22.1, 24.9};  // [degC]

    // 合計と平均。初期値は 0 ではなく 0.0（int にすると切り捨てバグ！）
    double temperature_sum =
        std::accumulate(temperatures.begin(), temperatures.end(), 0.0);
    std::cout << "mean: " << temperature_sum / temperatures.size() << "\n";

    // 最大・最小はイテレータが返るので * で値を取り出す
    double max_temperature =
        *std::max_element(temperatures.begin(), temperatures.end());
    double min_temperature =
        *std::min_element(temperatures.begin(), temperatures.end());
    std::cout << "max: " << max_temperature << ", min: " << min_temperature
              << "\n";

    std::sort(temperatures.begin(), temperatures.end());
    for (double temperature : temperatures) std::cout << temperature << " ";
    std::cout << "\n";
    return 0;
}
