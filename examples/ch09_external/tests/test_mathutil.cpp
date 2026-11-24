// mathutil のテスト。全 assert を通過して終了コード 0 なら合格。
// 実行方法:  ctest --test-dir build --output-on-failure
#include <cassert>
#include <cmath>
#include <iostream>

#include "mathutil.hpp"

constexpr double kPi = 3.14159265358979323846;

// 浮動小数点は「ぴったり一致」ではなく「誤差以内」で比較する
bool IsNear(double actual, double expected, double tolerance = 1e-6) {
    return std::abs(actual - expected) < tolerance;
}

int main() {
    // 台形則: sin の [0, π] 積分は 2
    assert(IsNear(IntegrateSinTrapezoid(0.0, kPi, 10000), 2.0, 1e-4));

    // 平均: {1,2,3} の平均は 2
    assert(IsNear(Mean({1.0, 2.0, 3.0}), 2.0));

    // 空データの平均は nullopt になるべき
    assert(!SafeMean({}).has_value());
    assert(IsNear(SafeMean({5.0}).value(), 5.0));

    // 演習1: RootMeanSquare({3,4}) = sqrt((9+16)/2) = sqrt(12.5)
    assert(IsNear(RootMeanSquare({3.0, 4.0}), std::sqrt(12.5)));

    std::cout << "all tests passed\n";
    return 0;  // ここまで到達 = 全部合格
}
