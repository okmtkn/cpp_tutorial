// 第3章 演習2: 平均と標準偏差を pair で返す
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

std::pair<double, double> MeanAndStddev(const std::vector<double>& values) {
    double sum = 0.0;
    for (double value : values) sum += value;
    const double mean = sum / values.size();

    double squared_deviation_sum = 0.0;
    for (double value : values) {
        squared_deviation_sum += (value - mean) * (value - mean);
    }
    const double stddev = std::sqrt(squared_deviation_sum / values.size());  // 母標準偏差

    return {mean, stddev};
}

int main() {
    std::vector<double> pin_diameters = {2.1, 2.4, 1.9, 2.2, 2.0, 2.3};  // ピンの直径 [mm]

    auto [mean_diameter, stddev_diameter] = MeanAndStddev(pin_diameters);
    std::cout << "mean = " << mean_diameter << " mm, stddev = " << stddev_diameter
              << " mm\n";
    return 0;
}
