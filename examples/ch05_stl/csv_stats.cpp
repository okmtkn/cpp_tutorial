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

// センサの1回分の計測値（CSV の1行に対応）
struct SensorSample {
    double time;          // [s]
    double temperature;   // [degC]
    double acceleration;  // [m/s^2]
};

std::vector<SensorSample> LoadSensorCsv(const std::string& csv_path) {
    std::ifstream csv_file(csv_path);
    if (!csv_file) throw std::runtime_error("cannot open: " + csv_path);

    std::vector<SensorSample> samples;
    std::string line;
    std::getline(csv_file, line);  // ヘッダ行を読み飛ばす

    while (std::getline(csv_file, line)) {
        std::stringstream line_stream(line);
        std::string cell;
        SensorSample sample{};

        std::getline(line_stream, cell, ',');
        sample.time = std::stod(cell);
        std::getline(line_stream, cell, ',');
        sample.temperature = std::stod(cell);
        std::getline(line_stream, cell, ',');
        sample.acceleration = std::stod(cell);

        samples.push_back(sample);
    }
    return samples;
}

int main() {
    const std::string csv_path = std::string(DATA_DIR) + "/sensor.csv";
    std::vector<SensorSample> samples = LoadSensorCsv(csv_path);
    std::cout << "loaded " << samples.size() << " samples\n";

    // 温度の平均（accumulate + ラムダで各サンプルの temperature を取り出しながら足す）
    double temperature_sum = std::accumulate(
        samples.begin(), samples.end(), 0.0,
        [](double partial_sum, const SensorSample& sample) {
            return partial_sum + sample.temperature;
        });
    std::cout << "mean temp: " << temperature_sum / samples.size() << " degC\n";

    // 加速度が最大のサンプル（衝撃の瞬間）を探す
    auto peak_sample = std::max_element(
        samples.begin(), samples.end(),
        [](const SensorSample& lhs, const SensorSample& rhs) {
            return lhs.acceleration < rhs.acceleration;
        });
    std::cout << "peak: t = " << peak_sample->time
              << " s, accel = " << peak_sample->acceleration << " m/s^2\n";
    return 0;
}
