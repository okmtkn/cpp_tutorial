// 3.3 値渡し・参照渡し・const参照渡し / 3.4 vector を返す関数
#include <iostream>
#include <vector>

// (1) 値渡し: 小さい型はコピーで渡す
double half(double x) { return x / 2.0; }

// (2) 参照渡し: 呼び出し元のデータを書き換える
void scale(std::vector<double>& v, double factor) {
    for (double& x : v) x *= factor;
}

// (3) const参照渡し: 大きなデータを読むだけ（コピーを避ける）
double sum(const std::vector<double>& v) {
    double s = 0.0;
    for (double x : v) s += x;
    return s;
}

// vector はそのまま return できる: 0 から end まで step 刻みの数列
std::vector<double> linspace_step(double end, double step) {
    std::vector<double> result;
    for (double x = 0.0; x <= end; x += step) {
        result.push_back(x);
    }
    return result;
}

int main() {
    std::vector<double> data = {1.0, 2.0, 3.0};
    scale(data, 10.0);
    std::cout << "sum = " << sum(data) << "\n";        // 60
    std::cout << "half = " << half(sum(data)) << "\n"; // 30

    auto t = linspace_step(1.0, 0.25);
    for (double x : t) std::cout << x << " ";  // 0 0.25 0.5 0.75 1
    std::cout << "\n";
    return 0;
}
