// 第2章 演習2: 名前の逆順表示
// ポイント: std::size_t は 0 未満にならない（符号なし整数）ので、
// i >= 0 を終了条件にすると無限ループになる。i の減算タイミングを工夫する。
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> student_names = {"Sato", "Suzuki", "Takahashi",
                                              "Tanaka", "Watanabe"};

    // 書き方1: i を 1 始まりで数えて student_names[size - i] を見る
    for (std::size_t i = 1; i <= student_names.size(); ++i) {
        std::cout << student_names[student_names.size() - i] << "\n";
    }

    // 書き方2 (参考): 逆イテレータを使うとより簡潔
    // for (auto it = student_names.rbegin(); it != student_names.rend(); ++it)
    //     std::cout << *it << "\n";
    return 0;
}
