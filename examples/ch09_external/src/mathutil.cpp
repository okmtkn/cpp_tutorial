#include "mathutil.hpp"

#include <cmath>

double trapezoid_sin(double a, double b, int n) {
    const double h = (b - a) / n;
    double s = 0.5 * (std::sin(a) + std::sin(b));
    for (int i = 1; i < n; ++i) {
        s += std::sin(a + i * h);
    }
    return s * h;
}

double mean(const std::vector<double>& v) {
    double sum = 0.0;
    for (double x : v) sum += x;
    return sum / v.size();
}

std::optional<double> mean_safe(const std::vector<double>& v) {
    if (v.empty()) return std::nullopt;
    return mean(v);
}

double rms(const std::vector<double>& v) {
    double sq = 0.0;
    for (double x : v) sq += x * x;
    return std::sqrt(sq / v.size());
}
