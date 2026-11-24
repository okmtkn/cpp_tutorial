// numlib を使うアプリ本体
#include <cmath>
#include <iostream>
#include <vector>

#include "numlib/integrate.hpp"
#include "numlib/statistics.hpp"

int main() {
    // sin(x) を [0, π] で積分（理論値 2.0）
    constexpr double kPi = 3.14159265358979323846;
    const double sin_integral = numlib::IntegrateTrapezoid(
        [](double x) { return std::sin(x); }, 0.0, kPi);
    std::cout << "integral of sin on [0, pi] = " << sin_integral << "\n";

    std::vector<double> pin_diameters = {2.1, 2.4, 1.9, 2.2, 2.0};  // ピンの直径 [mm]
    auto [mean_diameter, stddev_diameter] = numlib::MeanAndStddev(pin_diameters);
    std::cout << "mean = " << mean_diameter << ", stddev = " << stddev_diameter
              << "\n";

    auto [min_diameter, max_diameter] = numlib::MinMax(pin_diameters);
    std::cout << "min = " << min_diameter << ", max = " << max_diameter << "\n";
    return 0;
}
