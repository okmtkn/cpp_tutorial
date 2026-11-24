// 2.2 std::string — char配列とstring.hの置き換え
#include <iostream>
#include <string>

int main() {
    std::string name = "Kosen Taro";
    std::string greeting = "Hello, " + name + "!";  // + で連結

    std::cout << greeting << "\n";
    std::cout << "length: " << greeting.size() << "\n";
    std::cout << "first char: " << greeting[0] << "\n";

    if (name == "Kosen Taro") {  // strcmp ではなく == で比較
        std::cout << "name matched\n";
    } else {
        std::cout << "name not matched\n";
    }

    std::string line;
    std::cout << "Type one line: ";
    std::getline(std::cin, line);  // 空白を含む1行を読む
    std::cout << "you typed: " << line << "\n";
    return 0;
}
