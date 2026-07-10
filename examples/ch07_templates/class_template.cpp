// 7.3 クラステンプレート — vector<T> や pair<A,B> と同じ仕組み
#include <iostream>
#include <string>

// 測定値と単位をセットで持つ型
template <typename T>
struct Measured {
    T value;
    std::string unit;

    void print() const { std::cout << value << " [" << unit << "]\n"; }
};

int main() {
    Measured<double> temp{23.5, "degC"};
    Measured<int> count{42, "cycles"};

    temp.print();
    count.print();
    return 0;
}
