// 7.1〜7.2 関数テンプレート
#include <iostream>
#include <string>
#include <vector>

// 任意の型 T について MaxOf を定義する
template <typename T>
T MaxOf(T lhs, T rhs) {
    return (lhs > rhs) ? lhs : rhs;
}

// 「+= できる型」の vector なら何でも合計できる
template <typename T>
T Sum(const std::vector<T>& values) {
    T total{};  // T{} は「その型のゼロ値」(0, 0.0, 空文字列...)
    for (const T& value : values) {
        total += value;
    }
    return total;
}

int main() {
    std::cout << MaxOf(3, 7) << "\n";      // T = int
    std::cout << MaxOf(2.5, 1.4) << "\n";  // T = double
    std::cout << MaxOf(std::string("abc"), std::string("abd")) << "\n";

    std::vector<int> daily_part_counts = {1, 2, 3};         // 日ごとの部品数 [個]
    std::vector<double> weight_masses = {0.5, 0.25};        // おもりの質量 [kg]
    std::vector<std::string> code_parts = {"ab", "cd", "ef"};  // 部品コードの断片

    std::cout << Sum(daily_part_counts) << "\n";  // 6
    std::cout << Sum(weight_masses) << "\n";      // 0.75
    std::cout << Sum(code_parts) << "\n";         // abcdef（+= が連結だから）
    return 0;
}
