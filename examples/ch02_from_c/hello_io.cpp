// 2.1 入出力: printf/scanf の代わりに cout/cin を使う
#include <iostream>

int main() {
    int age;
    std::cout << "Enter your age: ";
    std::cin >> age;
    std::cout << "You are " << age << " years old.\n";
    return 0;
}
