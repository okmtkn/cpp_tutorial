// 2.4 範囲for / 2.5 auto / 2.6 参照
#include <iostream>
#include <vector>

// 参照渡し: 呼び出し元の変数そのものを書き換える（ポインタ不要）
void increment(int& n) { n++; }

int main() {
    std::vector<double> temps = {23.5, 24.1, 22.8, 25.0};  // 摂氏 [degC]

    // 読むだけの範囲for
    for (double t : temps) std::cout << t << " ";
    std::cout << "\n";

    // 書き換える範囲for（& を付ける）: 摂氏 → 絶対温度
    for (double& t : temps) t += 273.15;

    // auto: 右辺から型を推論
    for (auto t : temps) std::cout << t << " ";
    std::cout << "\n";

    int a = 10;
    increment(a);  // & も * も書かずに a が書き換わる
    std::cout << "a = " << a << "\n";  // 11
    return 0;
}
