// 3.5 複数の値を返す — std::pair と構造化束縛
#include <iostream>
#include <utility>
#include <vector>

// 最小値と最大値を同時に返す
std::pair<double, double> min_max(const std::vector<double>& v) {
    double lo = v[0], hi = v[0];
    for (double x : v) {
        if (x < lo) lo = x;
        if (x > hi) hi = x;
    }
    return {lo, hi};
}

int main() {
    std::vector<double> stress = {120.5, 98.2, 141.7, 110.3};  // 応力 [MPa]

    auto [lo, hi] = min_max(stress);  // 構造化束縛で2変数に受け取る
    std::cout << "min: " << lo << " MPa, max: " << hi << " MPa\n";
    return 0;
}
