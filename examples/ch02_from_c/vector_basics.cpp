// 2.3 std::vector — サイズを自動管理する可変長配列
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {10, 20, 30};

    v.push_back(40);  // 末尾に追加（メモリは自動で伸びる）
    v.push_back(50);

    std::cout << "size : " << v.size() << "\n";
    std::cout << "front: " << v[0] << ", back: " << v.back() << "\n";

    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";
    return 0;
}
