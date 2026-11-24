// 7.3 クラステンプレート — vector<T> や pair<A,B> と同じ仕組み
#include <iostream>
#include <string>

// 測定値と単位をセットで持つ型
template <typename T>
struct Measured {
    T value;
    std::string unit;

    void Print() const { std::cout << value << " [" << unit << "]\n"; }
};

int main() {
    Measured<double> room_temperature{23.5, "degC"};
    Measured<int> load_cycle_count{42, "cycles"};  // 疲労試験の負荷サイクル数

    room_temperature.Print();
    load_cycle_count.Print();
    return 0;
}
