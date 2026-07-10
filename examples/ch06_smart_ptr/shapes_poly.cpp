// 6.4 多態性 — 基底クラスのポインタで異なる型を一括で扱う
#include <iostream>
#include <memory>
#include <vector>

class Shape {
public:
    virtual ~Shape() = default;       // 基底クラスのデストラクタは virtual に
    virtual double area() const = 0;  // 純粋仮想関数
    virtual const char* name() const = 0;
};

class Circle : public Shape {
public:
    Circle(double r) : r_(r) {}
    double area() const override { return 3.14159265358979 * r_ * r_; }
    const char* name() const override { return "Circle"; }

private:
    double r_;
};

class Rect : public Shape {
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    const char* name() const override { return "Rect"; }

private:
    double w_, h_;
};

int main() {
    // 「Shape の unique_ptr」の vector なら違う型を混ぜて入れられる
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rect>(2.0, 3.0));
    shapes.push_back(std::make_unique<Circle>(0.5));

    double total = 0.0;
    for (const auto& s : shapes) {
        std::cout << s->name() << ": " << s->area() << "\n";
        total += s->area();
    }
    std::cout << "total area: " << total << "\n";
    return 0;
}  // vector 破棄時に各 unique_ptr が中身を自動 delete
