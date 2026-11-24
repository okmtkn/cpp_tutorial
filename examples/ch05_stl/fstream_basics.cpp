// 5.4 ファイル入出力 — ofstream / ifstream（close は自動 = RAII）
#include <fstream>
#include <iostream>
#include <string>

int main() {
    // 書き込み: 1〜5 とその2乗の表
    {
        std::ofstream output_file("result.txt");
        output_file << "x, x^2\n";
        for (int value = 1; value <= 5; ++value) {
            output_file << value << ", " << value * value << "\n";
        }
    }  // スコープを抜けると自動で閉じる（close() 不要）

    // 読み込み
    std::ifstream input_file("result.txt");
    if (!input_file) {  // 開けたかのチェックは必須
        std::cerr << "cannot open result.txt\n";
        return 1;
    }
    std::string line;
    while (std::getline(input_file, line)) {
        std::cout << line << "\n";
    }
    return 0;
}
