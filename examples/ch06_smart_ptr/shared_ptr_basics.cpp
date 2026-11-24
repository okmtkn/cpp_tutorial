// 6.5 shared_ptr — 参照カウントで共有される所有権
#include <iostream>
#include <memory>

int main() {
    // 生産数のデータを、生産管理と監視モニタの2か所で共有する例
    std::shared_ptr<int> production_count = std::make_shared<int>(42);
    std::cout << "use_count = " << production_count.use_count() << "\n";  // 1
    {
        std::shared_ptr<int> monitor_view = production_count;  // コピーOK: 所有者が2人に
        std::cout << "use_count = " << production_count.use_count() << "\n";  // 2
        std::cout << "*monitor_view = " << *monitor_view << "\n";
    }  // monitor_view 消滅
    std::cout << "use_count = " << production_count.use_count() << "\n";  // 1
    return 0;
}  // production_count 消滅 → ここで解放される
