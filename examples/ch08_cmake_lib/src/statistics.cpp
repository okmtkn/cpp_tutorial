#include "numlib/statistics.hpp"

#include <cmath>
#include <stdexcept>

namespace numlib {

std::pair<double, double> MeanAndStddev(const std::vector<double>& values) {
    if (values.empty()) throw std::invalid_argument("empty data");

    double sum = 0.0;
    for (double value : values) sum += value;
    const double mean = sum / values.size();

    double squared_deviation_sum = 0.0;
    for (double value : values) {
        squared_deviation_sum += (value - mean) * (value - mean);
    }
    return {mean, std::sqrt(squared_deviation_sum / values.size())};
}

std::pair<double, double> MinMax(const std::vector<double>& values) {
    if (values.empty()) throw std::invalid_argument("empty data");

    double min_value = values[0];
    double max_value = values[0];
    for (double value : values) {
        if (value < min_value) min_value = value;
        if (value > max_value) max_value = value;
    }
    return {min_value, max_value};
}

}  // namespace numlib
