#include "numlib/integrate.hpp"

namespace numlib {

double trapezoid(const std::function<double(double)>& f,
                 double a, double b, int n) {
    const double h = (b - a) / n;
    double s = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; ++i) {
        s += f(a + i * h);
    }
    return s * h;
}

}  // namespace numlib
