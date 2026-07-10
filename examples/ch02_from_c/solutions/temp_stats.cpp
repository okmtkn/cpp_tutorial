// 第2章 演習1: 温度データの平均・最大・最小
#include <iostream>
#include <vector>

int main() {
    std::vector<double> temps = {23.5, 24.1, 22.8, 25.0, 21.9,
                                 26.3, 23.0, 24.8, 22.5, 25.5};

    double sum = 0.0;
    double hi = temps[0];  // 最初の要素で初期化するのがコツ
    double lo = temps[0];

    for (double t : temps) {
        sum += t;
        if (t > hi) hi = t;
        if (t < lo) lo = t;
    }

    std::cout << "mean: " << sum / temps.size() << "\n";
    std::cout << "max : " << hi << "\n";
    std::cout << "min : " << lo << "\n";
    return 0;
}
