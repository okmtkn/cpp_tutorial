// 2.3 std::vector — サイズを自動管理する可変長配列
#include <iostream>
#include <vector>

int main() {
    std::vector<int> bolt_lengths = {10, 20, 30};  // ボルトの長さ [mm]

    bolt_lengths.push_back(40);  // 末尾に追加（メモリは自動で伸びる）
    bolt_lengths.push_back(50);

    std::cout << "size : " << bolt_lengths.size() << "\n";
    std::cout << "front: " << bolt_lengths[0]
              << ", back: " << bolt_lengths.back() << "\n";

    for (std::size_t i = 0; i < bolt_lengths.size(); ++i) {
        std::cout << bolt_lengths[i] << " ";
    }
    std::cout << "\n";
    return 0;
}
