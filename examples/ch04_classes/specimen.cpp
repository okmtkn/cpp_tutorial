// 4.3 private / public — 不正な状態を作らせないクラス設計
#include <iostream>
#include <stdexcept>

// 材料試験片: 断面積は正の値しか許さない
class Specimen {
public:
    explicit Specimen(double area_mm2) : area_mm2_(area_mm2) {
        if (area_mm2 <= 0.0) {
            throw std::invalid_argument("area must be positive");
        }
    }

    // 応力 [MPa] = 荷重 [N] / 断面積 [mm^2]
    double Stress(double load_newton) const { return load_newton / area_mm2_; }

    double area_mm2() const { return area_mm2_; }  // 読み取り用（getter）

private:
    double area_mm2_;  // 外から直接書き換えられない
};

int main() {
    Specimen steel_specimen(25.0);  // 5mm x 5mm の試験片
    std::cout << "stress: " << steel_specimen.Stress(5000.0)
              << " MPa\n";  // 200 MPa

    // steel_specimen.area_mm2_ = -1.0;  // ← コメントを外すとコンパイルエラーになる

    try {
        Specimen invalid_specimen(-3.0);  // 不正な値はコンストラクタが拒否する
    } catch (const std::exception& error) {
        std::cout << "rejected: " << error.what() << "\n";
    }
    return 0;
}
