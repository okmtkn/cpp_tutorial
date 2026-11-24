// 6.1 RAII — デストラクタはスコープを抜けるとき自動で呼ばれる
#include <iostream>

// 生成と破棄のタイミングを画面に表示するだけのクラス
class LifetimeLogger {
public:
    LifetimeLogger(const char* label) : label_(label) {
        std::cout << label_ << ": constructed\n";
    }
    ~LifetimeLogger() {  // デストラクタ
        std::cout << label_ << ": destructed\n";
    }

private:
    const char* label_;
};

int main() {
    LifetimeLogger outer_logger("outer");
    {
        LifetimeLogger inner_logger("inner");
        std::cout << "--- inner scope end ---\n";
    }  // ここで inner_logger のデストラクタが自動で呼ばれる
    std::cout << "--- main end ---\n";
    return 0;
}  // ここで outer_logger のデストラクタが自動で呼ばれる
