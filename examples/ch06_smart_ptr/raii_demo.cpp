// 6.1 RAII — デストラクタはスコープを抜けるとき自動で呼ばれる
#include <iostream>

class Probe {
public:
    Probe(const char* name) : name_(name) {
        std::cout << name_ << ": constructed\n";
    }
    ~Probe() {  // デストラクタ
        std::cout << name_ << ": destructed\n";
    }

private:
    const char* name_;
};

int main() {
    Probe a("a");
    {
        Probe b("b");
        std::cout << "--- inner scope end ---\n";
    }  // ここで b のデストラクタが自動で呼ばれる
    std::cout << "--- main end ---\n";
    return 0;
}  // ここで a のデストラクタが自動で呼ばれる
