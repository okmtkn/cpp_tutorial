// 6.3 unique_ptr — 自動で delete されるポインタと所有権の移動
#include <iostream>
#include <memory>
#include <utility>

class Motor {
public:
    Motor(int id) : id_(id) { std::cout << "Motor " << id_ << " ON\n"; }
    ~Motor() { std::cout << "Motor " << id_ << " OFF\n"; }
    void Drive() const { std::cout << "Motor " << id_ << " driving\n"; }

private:
    int id_;
};

int main() {
    // new の代わりに make_unique（倉庫に在庫のモータが1台ある）
    std::unique_ptr<Motor> stock_motor = std::make_unique<Motor>(1);
    stock_motor->Drive();

    // unique_ptr はコピーできない（所有者は常に1人）
    // std::unique_ptr<Motor> installed_motor = stock_motor;  // コンパイルエラー
    // 在庫から装置へ取り付ける = 所有権の移動はOK
    std::unique_ptr<Motor> installed_motor = std::move(stock_motor);
    installed_motor->Drive();

    if (!stock_motor) {
        std::cout << "stock_motor is now empty (nullptr)\n";
    }
    return 0;
}  // installed_motor がスコープを抜けるとき自動で delete される
