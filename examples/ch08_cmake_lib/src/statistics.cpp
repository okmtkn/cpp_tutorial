#include "numlib/statistics.hpp"

#include <cmath>
#include <stdexcept>

namespace numlib {

std::pair<double, double> mean_stddev(const std::vector<double>& v) {
    if (v.empty()) throw std::invalid_argument("empty data");

    double sum = 0.0;
    for (double x : v) sum += x;
    const double mean = sum / v.size();

    double sq = 0.0;
    for (double x : v) sq += (x - mean) * (x - mean);
    return {mean, std::sqrt(sq / v.size())};
}

std::pair<double, double> min_max(const std::vector<double>& v) {
    if (v.empty()) throw std::invalid_argument("empty data");

    double lo = v[0], hi = v[0];
    for (double x : v) {
        if (x < lo) lo = x;
        if (x > hi) hi = x;
    }
    return {lo, hi};
}

}  // namespace numlib
