// 6.3 unique_ptr — 自動で delete されるポインタと所有権の移動
#include <iostream>
#include <memory>
#include <utility>

class Motor {
public:
    Motor(int id) : id_(id) { std::cout << "Motor " << id_ << " ON\n"; }
    ~Motor() { std::cout << "Motor " << id_ << " OFF\n"; }
    void drive() const { std::cout << "Motor " << id_ << " driving\n"; }

private:
    int id_;
};

int main() {
    // new の代わりに make_unique
    std::unique_ptr<Motor> a = std::make_unique<Motor>(1);
    a->drive();

    // unique_ptr はコピーできない（所有者は常に1人）
    // std::unique_ptr<Motor> b = a;         // コンパイルエラー
    std::unique_ptr<Motor> b = std::move(a);  // 所有権の移動はOK
    b->drive();

    if (!a) {
        std::cout << "a is now empty (nullptr)\n";
    }
    return 0;
}  // b がスコープを抜けるとき自動で delete される
