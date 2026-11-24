// 第2章 演習3 (発展): "end" と入力されるまで行を貯めて、最後に全件表示
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> lines;
    std::string line;

    std::cout << "Type lines ('end' to finish):\n";
    while (std::getline(std::cin, line)) {
        if (line == "end") break;
        lines.push_back(line);
    }

    std::cout << "---- " << lines.size() << " line(s) ----\n";
    for (const std::string& stored_line : lines) {  // 読むだけなので const 参照
        std::cout << stored_line << "\n";
    }
    return 0;
}
