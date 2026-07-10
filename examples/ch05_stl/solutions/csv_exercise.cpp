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

struct Sample {
    double time;
    double temp;
    double accel;
};

std::vector<Sample> load_csv(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open: " + path);

    std::vector<Sample> samples;
    std::string line;
    std::getline(in, line);  // ヘッダを捨てる
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
    auto samples = load_csv(path);

    // --- 演習1: 温度の平均・最大・最小 ---
    double t_sum = std::accumulate(
        samples.begin(), samples.end(), 0.0,
        [](double acc, const Sample& s) { return acc + s.temp; });
    const double t_mean = t_sum / samples.size();

    auto [t_lo, t_hi] = std::minmax_element(
        samples.begin(), samples.end(),
        [](const Sample& a, const Sample& b) { return a.temp < b.temp; });

    std::cout << "temp mean: " << t_mean << " degC\n";
    std::cout << "temp max : " << t_hi->temp << " degC\n";
    std::cout << "temp min : " << t_lo->temp << " degC\n";

    // --- 演習1: |accel| > 1.0 の時刻一覧 ---
    std::cout << "|accel| > 1.0 at: ";
    for (const Sample& s : samples) {
        if (std::abs(s.accel) > 1.0) std::cout << s.time << "s ";
    }
    std::cout << "\n";

    // --- 演習2: 加速度の標準偏差 ---
    double a_sum = std::accumulate(
        samples.begin(), samples.end(), 0.0,
        [](double acc, const Sample& s) { return acc + s.accel; });
    const double a_mean = a_sum / samples.size();
    double sq = std::accumulate(
        samples.begin(), samples.end(), 0.0,
        [a_mean](double acc, const Sample& s) {
            return acc + (s.accel - a_mean) * (s.accel - a_mean);
        });
    const double a_sd = std::sqrt(sq / samples.size());
    std::cout << "accel stddev: " << a_sd << " m/s^2\n";

    // --- 演習3 (発展): 集計結果を CSV に書き出す ---
    std::ofstream out("summary.csv");
    out << "item,value\n";
    out << "temp_mean," << t_mean << "\n";
    out << "temp_max," << t_hi->temp << "\n";
    out << "temp_min," << t_lo->temp << "\n";
    out << "accel_stddev," << a_sd << "\n";
    std::cout << "wrote summary.csv\n";
    return 0;
}
