// 第7章 演習3 (発展): 時刻 query_time の温度を線形補間で求める（範囲外は nullopt）
#include <iostream>
#include <optional>
#include <vector>

struct TemperatureSample {
    double time;         // [s]
    double temperature;  // [degC]
};

std::optional<double> TemperatureAt(const std::vector<TemperatureSample>& samples,
                                    double query_time) {
    if (samples.empty() || query_time < samples.front().time ||
        query_time > samples.back().time) {
        return std::nullopt;  // データ範囲外
    }
    // query_time を挟む2点を探して線形補間
    for (std::size_t i = 0; i + 1 < samples.size(); ++i) {
        const TemperatureSample& before = samples[i];
        const TemperatureSample& after = samples[i + 1];
        if (before.time <= query_time && query_time <= after.time) {
            const double ratio =
                (query_time - before.time) / (after.time - before.time);
            return before.temperature +
                   ratio * (after.temperature - before.temperature);
        }
    }
    return std::nullopt;
}

int main() {
    std::vector<TemperatureSample> samples = {
        {0.0, 23.5}, {0.5, 24.6}, {1.0, 25.1}, {1.5, 24.3},
    };

    for (double query_time : {0.25, 0.75, 2.0}) {
        auto temperature = TemperatureAt(samples, query_time);
        if (temperature) {
            std::cout << "t = " << query_time << " s: " << *temperature
                      << " degC\n";
        } else {
            std::cout << "t = " << query_time << " s: out of range\n";
        }
    }
    return 0;
}
