// 第3章 演習1, 3: 台形則による sin(x) の数値積分
#include <cmath>
#include <iostream>

constexpr double PI = 3.14159265358979323846;

// 区間 [a, b] を n 分割した台形則で sin(x) を積分する
double trapezoid(double a, double b, int n) {
    const double h = (b - a) / n;
    double s = 0.5 * (std::sin(a) + std::sin(b));
    for (int i = 1; i < n; ++i) {
        s += std::sin(a + i * h);
    }
    return s * h;
}

// 演習3: オーバーロード版（n を省略したら 1000 分割）
// ※ この程度なら「デフォルト引数 int n = 1000」の方がシンプルで良い。
//    オーバーロードは「引数の型や処理自体が変わる」ときに使うのが適切。
double trapezoid(double a, double b) { return trapezoid(a, b, 1000); }

int main() {
    const double exact = 2.0;  // ∫[0,π] sin(x) dx = 2

    for (int n : {10, 100, 1000}) {
        double approx = trapezoid(0.0, PI, n);
        std::cout << "n = " << n << ": " << approx
                  << "  (error = " << std::abs(approx - exact) << ")\n";
    }
    std::cout << "default n: " << trapezoid(0.0, PI) << "\n";
    return 0;
}
