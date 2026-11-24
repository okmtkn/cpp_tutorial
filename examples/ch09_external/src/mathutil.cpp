#include "mathutil.hpp"

#include <cmath>

double IntegrateSinTrapezoid(double lower_limit, double upper_limit,
                             int num_intervals) {
    const double width = (upper_limit - lower_limit) / num_intervals;
    double sum = 0.5 * (std::sin(lower_limit) + std::sin(upper_limit));
    for (int i = 1; i < num_intervals; ++i) {
        sum += std::sin(lower_limit + i * width);
    }
    return sum * width;
}

double Mean(const std::vector<double>& values) {
    double sum = 0.0;
    for (double value : values) sum += value;
    return sum / values.size();
}

std::optional<double> SafeMean(const std::vector<double>& values) {
    if (values.empty()) return std::nullopt;
    return Mean(values);
}

double RootMeanSquare(const std::vector<double>& values) {
    double square_sum = 0.0;
    for (double value : values) square_sum += value * value;
    return std::sqrt(square_sum / values.size());
}
