// 3.3 値渡し・参照渡し・const参照渡し / 3.4 vector を返す関数
#include <cmath>
#include <iostream>
#include <vector>

// (1) 値渡し: 小さい型はコピーで渡す
double Half(double value) { return value / 2.0; }

// (2) 参照渡し: 呼び出し元のデータを書き換える
void ScaleAll(std::vector<double>& values, double factor) {
    for (double& value : values) value *= factor;
}

// (3) const参照渡し: 大きなデータを読むだけ（コピーを避ける）
double Sum(const std::vector<double>& values) {
    double total = 0.0;
    for (double value : values) total += value;
    return total;
}

// vector はそのまま return できる: 0 から end_time まで time_step 刻みの時刻列
std::vector<double> MakeTimePoints(double end_time, double time_step) {
    // 小数を足し続けると誤差がたまるので、刻みの回数を先に整数で求める
    const int num_steps = static_cast<int>(std::round(end_time / time_step));
    std::vector<double> time_points;
    for (int i = 0; i <= num_steps; ++i) {
        time_points.push_back(i * time_step);
    }
    return time_points;
}

int main() {
    std::vector<double> lengths = {1.0, 2.0, 3.0};  // 部材の長さ [cm]
    ScaleAll(lengths, 10.0);                         // cm → mm に換算
    std::cout << "sum = " << Sum(lengths) << " mm\n";         // 60
    std::cout << "half = " << Half(Sum(lengths)) << " mm\n";  // 30

    auto time_points = MakeTimePoints(1.0, 0.25);
    for (double time : time_points) std::cout << time << " ";  // 0 0.25 0.5 0.75 1
    std::cout << "\n";
    return 0;
}
