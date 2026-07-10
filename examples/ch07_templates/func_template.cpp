// 7.1〜7.2 関数テンプレート
#include <iostream>
#include <string>
#include <vector>

// 任意の型 T について max_of を定義する
template <typename T>
T max_of(T a, T b) {
    return (a > b) ? a : b;
}

// 「+= できる型」の vector なら何でも合計できる
template <typename T>
T sum(const std::vector<T>& v) {
    T total{};  // T{} は「その型のゼロ値」(0, 0.0, 空文字列...)
    for (const T& x : v) {
        total += x;
    }
    return total;
}

int main() {
    std::cout << max_of(3, 7) << "\n";      // T = int
    std::cout << max_of(2.5, 1.4) << "\n";  // T = double
    std::cout << max_of(std::string("abc"), std::string("abd")) << "\n";

    std::vector<int> a = {1, 2, 3};
    std::vector<double> b = {0.5, 0.25};
    std::vector<std::string> c = {"ab", "cd", "ef"};

    std::cout << sum(a) << "\n";  // 6
    std::cout << sum(b) << "\n";  // 0.75
    std::cout << sum(c) << "\n";  // abcdef（+= が連結だから）
    return 0;
}
