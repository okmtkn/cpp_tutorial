// 6.5 shared_ptr — 参照カウントで共有される所有権
#include <iostream>
#include <memory>

int main() {
    std::shared_ptr<int> a = std::make_shared<int>(42);
    std::cout << "count = " << a.use_count() << "\n";  // 1
    {
        std::shared_ptr<int> b = a;  // コピーOK: 所有者が2人に
        std::cout << "count = " << a.use_count() << "\n";  // 2
        std::cout << "*b = " << *b << "\n";
    }  // b 消滅
    std::cout << "count = " << a.use_count() << "\n";  // 1
    return 0;
}  // a 消滅 → ここで解放される
