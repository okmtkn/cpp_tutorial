// 演習2: x^2, sin(x), e^x の [0,1] 積分値の表を表示する
// （ラムダの引数 x は数式 f(x) の x そのものなので、短い名前のままで読みやすい）
#include <cmath>
#include <iostream>

#include "numlib/integrate.hpp"

int main() {
    std::cout << "f(x)      integral on [0,1]\n";
    std::cout << "--------  -----------------\n";

    std::cout << "x^2       "
              << numlib::IntegrateTrapezoid([](double x) { return x * x; }, 0.0, 1.0)
              << "  (exact 1/3)\n";
    std::cout << "sin(x)    "
              << numlib::IntegrateTrapezoid([](double x) { return std::sin(x); },
                                            0.0, 1.0)
              << "  (exact 1-cos(1))\n";
    std::cout << "e^x       "
              << numlib::IntegrateTrapezoid([](double x) { return std::exp(x); },
                                            0.0, 1.0)
              << "  (exact e-1)\n";
    return 0;
}
