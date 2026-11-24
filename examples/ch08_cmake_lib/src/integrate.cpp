#include "numlib/integrate.hpp"

namespace numlib {

double IntegrateTrapezoid(const std::function<double(double)>& integrand,
                          double lower_limit, double upper_limit,
                          int num_intervals) {
    const double width = (upper_limit - lower_limit) / num_intervals;  // 台形1つの幅
    double sum = 0.5 * (integrand(lower_limit) + integrand(upper_limit));
    for (int i = 1; i < num_intervals; ++i) {
        sum += integrand(lower_limit + i * width);
    }
    return sum * width;
}

}  // namespace numlib
