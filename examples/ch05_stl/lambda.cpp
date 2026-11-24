// 5.2 ラムダ式 — algorithm に判定基準を渡す
#include <algorithm>
#include <iostream>
#include <vector>

// ステップ1: 普通の関数で判定基準を作る
// 応力が 120 MPa を超えていれば true を返す
bool IsOver120(double stress) {
    return stress > 120.0;
}

int main() {
    std::vector<double> stresses = {120.5, 98.2, 141.7, 110.3, 135.2};  // [MPa]

    // ステップ1: 普通の関数を count_if に渡す（関数名だけを書く。() は付けない）
    auto count_by_function =
        std::count_if(stresses.begin(), stresses.end(), IsOver120);
    std::cout << "over 120 MPa (function): " << count_by_function << "\n";

    // ステップ2: 同じ判定をラムダ式でその場に書く
    auto count_over_120 =
        std::count_if(stresses.begin(), stresses.end(),
                      [](double stress) { return stress > 120.0; });
    std::cout << "over 120 MPa (lambda):   " << count_over_120 << "\n";

    // ステップ3: ラムダ式は変数に入れて、普通の関数のように呼べる
    auto is_over_120 = [](double stress) { return stress > 120.0; };
    std::cout << "is_over_120(141.7): " << is_over_120(141.7) << "\n";  // 1 (true)
    std::cout << "is_over_120(98.2):  " << is_over_120(98.2) << "\n";   // 0 (false)

    // ステップ4: 引数2つのラムダ — 降順ソート
    std::sort(stresses.begin(), stresses.end(),
              [](double lhs, double rhs) { return lhs > rhs; });
    for (double stress : stresses) std::cout << stress << " ";
    std::cout << "\n";

    // ステップ5: 外の変数を捕獲して使う
    double stress_limit = 130.0;  // 許容応力 [MPa]
    auto count_over_limit = std::count_if(
        stresses.begin(), stresses.end(),
        [stress_limit](double stress) { return stress > stress_limit; });
    std::cout << "over " << stress_limit << " MPa: " << count_over_limit
              << "\n";
    return 0;
}
