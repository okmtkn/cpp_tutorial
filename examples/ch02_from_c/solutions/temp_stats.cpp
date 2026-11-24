// 第2章 演習1: 温度データの平均・最大・最小
#include <iostream>
#include <vector>

int main() {
    std::vector<double> temperatures = {23.5, 24.1, 22.8, 25.0, 21.9,
                                        26.3, 23.0, 24.8, 22.5, 25.5};  // [degC]

    double temperature_sum = 0.0;
    double max_temperature = temperatures[0];  // 最初の要素で初期化するのがコツ
    double min_temperature = temperatures[0];

    for (double temperature : temperatures) {
        temperature_sum += temperature;
        if (temperature > max_temperature) max_temperature = temperature;
        if (temperature < min_temperature) min_temperature = temperature;
    }

    std::cout << "mean: " << temperature_sum / temperatures.size() << "\n";
    std::cout << "max : " << max_temperature << "\n";
    std::cout << "min : " << min_temperature << "\n";
    return 0;
}
