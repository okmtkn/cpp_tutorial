// 第3章 演習1, 3: 台形則による sin(x) の数値積分
#include <cmath>
#include <iostream>

constexpr double kPi = 3.14159265358979323846;

// 区間 [lower_limit, upper_limit] を num_intervals 分割した台形則で sin(x) を積分する
double IntegrateSinTrapezoid(double lower_limit, double upper_limit,
                             int num_intervals) {
    const double width = (upper_limit - lower_limit) / num_intervals;  // 台形1つの幅
    double sum = 0.5 * (std::sin(lower_limit) + std::sin(upper_limit));
    for (int i = 1; i < num_intervals; ++i) {
        sum += std::sin(lower_limit + i * width);
    }
    return sum * width;
}

// 演習3: オーバーロード版（分割数を省略したら 1000 分割）
// ※ この程度なら「デフォルト引数 int num_intervals = 1000」の方がシンプルで良い。
//    オーバーロードは「引数の型や処理自体が変わる」ときに使うのが適切。
double IntegrateSinTrapezoid(double lower_limit, double upper_limit) {
    return IntegrateSinTrapezoid(lower_limit, upper_limit, 1000);
}

int main() {
    const double exact_integral = 2.0;  // ∫[0,π] sin(x) dx = 2

    for (int num_intervals : {10, 100, 1000}) {
        const double approx_integral =
            IntegrateSinTrapezoid(0.0, kPi, num_intervals);
        std::cout << "n = " << num_intervals << ": " << approx_integral
                  << "  (error = " << std::abs(approx_integral - exact_integral)
                  << ")\n";
    }
    std::cout << "default n: " << IntegrateSinTrapezoid(0.0, kPi) << "\n";
    return 0;
}
