// 5.2 ラムダ式 — algorithm に判定基準を渡す
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<double> stress = {120.5, 98.2, 141.7, 110.3, 135.2};  // [MPa]

    // 降順ソート: 比較関数をラムダ式で渡す
    std::sort(stress.begin(), stress.end(),
              [](double a, double b) { return a > b; });
    for (double x : stress) std::cout << x << " ";
    std::cout << "\n";

    // 条件を満たす要素数
    auto n = std::count_if(stress.begin(), stress.end(),
                           [](double x) { return x > 120.0; });
    std::cout << "over 120 MPa: " << n << "\n";

    // 変数を捕獲するラムダ
    double limit = 130.0;
    auto m = std::count_if(stress.begin(), stress.end(),
                           [limit](double x) { return x > limit; });
    std::cout << "over " << limit << " MPa: " << m << "\n";
    return 0;
}
