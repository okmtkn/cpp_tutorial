// 5.1 algorithm の定番: accumulate / max_element / min_element / sort
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<double> data = {23.5, 21.2, 25.8, 22.1, 24.9};

    // 合計と平均。初期値は 0 ではなく 0.0（int にすると切り捨てバグ！）
    double total = std::accumulate(data.begin(), data.end(), 0.0);
    std::cout << "mean: " << total / data.size() << "\n";

    // 最大・最小はイテレータが返るので * で値を取り出す
    double hi = *std::max_element(data.begin(), data.end());
    double lo = *std::min_element(data.begin(), data.end());
    std::cout << "max: " << hi << ", min: " << lo << "\n";

    std::sort(data.begin(), data.end());
    for (double x : data) std::cout << x << " ";
    std::cout << "\n";
    return 0;
}
