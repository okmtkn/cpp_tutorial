// 第7章 演習3 (発展): 時刻 t の温度を線形補間で求める（範囲外は nullopt）
#include <iostream>
#include <optional>
#include <vector>

struct Sample {
    double time;
    double temp;
};

std::optional<double> temp_at(const std::vector<Sample>& data, double t) {
    if (data.empty() || t < data.front().time || t > data.back().time) {
        return std::nullopt;  // データ範囲外
    }
    // t を挟む2点を探して線形補間
    for (std::size_t i = 0; i + 1 < data.size(); ++i) {
        const Sample& a = data[i];
        const Sample& b = data[i + 1];
        if (a.time <= t && t <= b.time) {
            const double ratio = (t - a.time) / (b.time - a.time);
            return a.temp + ratio * (b.temp - a.temp);
        }
    }
    return std::nullopt;
}

int main() {
    std::vector<Sample> data = {
        {0.0, 23.5}, {0.5, 24.6}, {1.0, 25.1}, {1.5, 24.3},
    };

    for (double t : {0.25, 0.75, 2.0}) {
        auto temp = temp_at(data, t);
        if (temp) {
            std::cout << "t = " << t << " s: " << *temp << " degC\n";
        } else {
            std::cout << "t = " << t << " s: out of range\n";
        }
    }
    return 0;
}
