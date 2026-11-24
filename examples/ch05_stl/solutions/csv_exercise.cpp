// 第5章 演習1〜3: CSV集計（平均/最大/最小・しきい値超え時刻・標準偏差・CSV出力）
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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
    std::getline(csv_file, line);  // ヘッダを捨てる
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
    auto samples = LoadSensorCsv(csv_path);

    // --- 演習1: 温度の平均・最大・最小 ---
    double temperature_sum = std::accumulate(
        samples.begin(), samples.end(), 0.0,
        [](double partial_sum, const SensorSample& sample) {
            return partial_sum + sample.temperature;
        });
    const double mean_temperature = temperature_sum / samples.size();

    auto [coldest_sample, hottest_sample] = std::minmax_element(
        samples.begin(), samples.end(),
        [](const SensorSample& lhs, const SensorSample& rhs) {
            return lhs.temperature < rhs.temperature;
        });

    std::cout << "temp mean: " << mean_temperature << " degC\n";
    std::cout << "temp max : " << hottest_sample->temperature << " degC\n";
    std::cout << "temp min : " << coldest_sample->temperature << " degC\n";

    // --- 演習1: |accel| > 1.0 の時刻一覧 ---
    constexpr double kAccelerationThreshold = 1.0;  // [m/s^2]
    std::cout << "|accel| > " << kAccelerationThreshold << " at: ";
    for (const SensorSample& sample : samples) {
        if (std::abs(sample.acceleration) > kAccelerationThreshold) {
            std::cout << sample.time << "s ";
        }
    }
    std::cout << "\n";

    // --- 演習2: 加速度の標準偏差 ---
    double acceleration_sum = std::accumulate(
        samples.begin(), samples.end(), 0.0,
        [](double partial_sum, const SensorSample& sample) {
            return partial_sum + sample.acceleration;
        });
    const double mean_acceleration = acceleration_sum / samples.size();
    double squared_deviation_sum = std::accumulate(
        samples.begin(), samples.end(), 0.0,
        [mean_acceleration](double partial_sum, const SensorSample& sample) {
            const double deviation = sample.acceleration - mean_acceleration;
            return partial_sum + deviation * deviation;
        });
    const double acceleration_stddev =
        std::sqrt(squared_deviation_sum / samples.size());
    std::cout << "accel stddev: " << acceleration_stddev << " m/s^2\n";

    // --- 演習3 (発展): 集計結果を CSV に書き出す ---
    std::ofstream summary_file("summary.csv");
    summary_file << "item,value\n";
    summary_file << "temp_mean," << mean_temperature << "\n";
    summary_file << "temp_max," << hottest_sample->temperature << "\n";
    summary_file << "temp_min," << coldest_sample->temperature << "\n";
    summary_file << "accel_stddev," << acceleration_stddev << "\n";
    std::cout << "wrote summary.csv\n";
    return 0;
}
