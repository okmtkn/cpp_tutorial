// 5.4 ファイル入出力 — ofstream / ifstream（close は自動 = RAII）
#include <fstream>
#include <iostream>
#include <string>

int main() {
    // 書き込み
    {
        std::ofstream out("result.txt");
        out << "x, x^2\n";
        for (int x = 1; x <= 5; ++x) {
            out << x << ", " << x * x << "\n";
        }
    }  // スコープを抜けると自動で閉じる（close() 不要）

    // 読み込み
    std::ifstream in("result.txt");
    if (!in) {  // 開けたかのチェックは必須
        std::cerr << "cannot open result.txt\n";
        return 1;
    }
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << "\n";
    }
    return 0;
}
