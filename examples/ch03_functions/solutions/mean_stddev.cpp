// 第3章 演習2: 平均と標準偏差を pair で返す
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

std::pair<double, double> mean_stddev(const std::vector<double>& v) {
    double sum = 0.0;
    for (double x : v) sum += x;
    const double mean = sum / v.size();

    double sq = 0.0;
    for (double x : v) sq += (x - mean) * (x - mean);
    const double stddev = std::sqrt(sq / v.size());  // 母標準偏差

    return {mean, stddev};
}

int main() {
    std::vector<double> data = {2.1, 2.4, 1.9, 2.2, 2.0, 2.3};

    auto [mean, sd] = mean_stddev(data);
    std::cout << "mean = " << mean << ", stddev = " << sd << "\n";
    return 0;
}
