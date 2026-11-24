// 6.4 多態性 — 基底クラスのポインタで異なる型を一括で扱う
#include <iostream>
#include <memory>
#include <vector>

constexpr double kPi = 3.14159265358979323846;

class Shape {
public:
    virtual ~Shape() = default;       // 基底クラスのデストラクタは virtual に
    virtual double Area() const = 0;  // 純粋仮想関数
    virtual const char* Name() const = 0;
};

class Circle : public Shape {
public:
    Circle(double radius) : radius_(radius) {}
    double Area() const override { return kPi * radius_ * radius_; }
    const char* Name() const override { return "Circle"; }

private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}
    double Area() const override { return width_ * height_; }
    const char* Name() const override { return "Rectangle"; }

private:
    double width_;
    double height_;
};

int main() {
    // 「Shape の unique_ptr」の vector なら違う型を混ぜて入れられる
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rectangle>(2.0, 3.0));
    shapes.push_back(std::make_unique<Circle>(0.5));

    double total_area = 0.0;
    for (const auto& shape : shapes) {
        std::cout << shape->Name() << ": " << shape->Area() << "\n";
        total_area += shape->Area();
    }
    std::cout << "total area: " << total_area << "\n";
    return 0;
}  // vector 破棄時に各 unique_ptr が中身を自動 delete
