// numlib を使うアプリ本体
#include <cmath>
#include <iostream>
#include <vector>

#include "numlib/integrate.hpp"
#include "numlib/statistics.hpp"

int main() {
    // sin(x) を [0, π] で積分（理論値 2.0）
    constexpr double PI = 3.14159265358979323846;
    double s = numlib::trapezoid([](double x) { return std::sin(x); }, 0.0, PI);
    std::cout << "integral of sin on [0, pi] = " << s << "\n";

    std::vector<double> data = {2.1, 2.4, 1.9, 2.2, 2.0};
    auto [mean, sd] = numlib::mean_stddev(data);
    std::cout << "mean = " << mean << ", stddev = " << sd << "\n";

    auto [lo, hi] = numlib::min_max(data);
    std::cout << "min = " << lo << ", max = " << hi << "\n";
    return 0;
}
