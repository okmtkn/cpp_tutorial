// 2.4 範囲for / 2.5 auto / 2.6 参照
#include <iostream>
#include <vector>

// 参照渡し: 呼び出し元の変数そのものを書き換える（ポインタ不要）
void Increment(int& count) { count++; }

int main() {
    std::vector<double> temperatures = {23.5, 24.1, 22.8, 25.0};  // 摂氏 [degC]

    // 読むだけの範囲for
    for (double temperature : temperatures) std::cout << temperature << " ";
    std::cout << "\n";

    // 書き換える範囲for（& を付ける）: 摂氏 → 絶対温度
    for (double& temperature : temperatures) temperature += 273.15;

    // auto: 右辺から型を推論
    for (auto temperature : temperatures) std::cout << temperature << " ";
    std::cout << "\n";

    int part_count = 10;     // 部品の個数
    Increment(part_count);   // & も * も書かずに part_count が書き換わる
    std::cout << "part_count = " << part_count << "\n";  // 11
    return 0;
}
