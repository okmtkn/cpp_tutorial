// 5.5 実践: CSVファイルの読み込みと集計
// DATA_DIR は CMakeLists.txt の target_compile_definitions で埋め込まれる
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Sample {
    double time;   // [s]
    double temp;   // [degC]
    double accel;  // [m/s^2]
};

std::vector<Sample> load_csv(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open: " + path);

    std::vector<Sample> samples;
    std::string line;
    std::getline(in, line);  // ヘッダ行を読み飛ばす

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string cell;
        Sample s{};

        std::getline(ss, cell, ',');
        s.time = std::stod(cell);
        std::getline(ss, cell, ',');
        s.temp = std::stod(cell);
        std::getline(ss, cell, ',');
        s.accel = std::stod(cell);

        samples.push_back(s);
    }
    return samples;
}

int main() {
    const std::string path = std::string(DATA_DIR) + "/sensor.csv";
    std::vector<Sample> samples = load_csv(path);
    std::cout << "loaded " << samples.size() << " samples\n";

    // 温度の平均（accumulate + ラムダで Sample から temp を取り出しながら足す）
    double t_sum = std::accumulate(
        samples.begin(), samples.end(), 0.0,
        [](double acc, const Sample& s) { return acc + s.temp; });
    std::cout << "mean temp: " << t_sum / samples.size() << " degC\n";

    // 加速度が最大のサンプル（衝撃の瞬間）を探す
    auto peak = std::max_element(
        samples.begin(), samples.end(),
        [](const Sample& a, const Sample& b) { return a.accel < b.accel; });
    std::cout << "peak: t = " << peak->time << " s, accel = " << peak->accel
              << " m/s^2\n";
    return 0;
}
