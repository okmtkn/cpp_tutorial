// 第6章 演習1〜3: Triangle 追加・最大面積の図形・周長 Perimeter
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

constexpr double kPi = 3.14159265358979323846;

class Shape {
public:
    virtual ~Shape() = default;
    virtual double Area() const = 0;
    virtual double Perimeter() const = 0;  // 演習3
    virtual const char* Name() const = 0;
};

class Circle : public Shape {
public:
    Circle(double radius) : radius_(radius) {}
    double Area() const override { return kPi * radius_ * radius_; }
    double Perimeter() const override { return 2.0 * kPi * radius_; }
    const char* Name() const override { return "Circle"; }

private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}
    double Area() const override { return width_ * height_; }
    double Perimeter() const override { return 2.0 * (width_ + height_); }
    const char* Name() const override { return "Rectangle"; }

private:
    double width_;
    double height_;
};

// 演習1: 直角三角形（直角をはさむ底辺と高さで表し、斜辺は三平方の定理で求める）
class Triangle : public Shape {
public:
    Triangle(double base, double height) : base_(base), height_(height) {}
    double Area() const override { return 0.5 * base_ * height_; }
    double Perimeter() const override {
        const double hypotenuse = std::sqrt(base_ * base_ + height_ * height_);
        return base_ + height_ + hypotenuse;
    }
    const char* Name() const override { return "Triangle"; }

private:
    double base_;
    double height_;
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rectangle>(2.0, 3.0));
    shapes.push_back(std::make_unique<Triangle>(3.0, 4.0));

    for (const auto& shape : shapes) {
        std::cout << shape->Name() << ": area = " << shape->Area()
                  << ", perimeter = " << shape->Perimeter() << "\n";
    }

    // 演習2: 面積が最大の図形を max_element + ラムダで探す
    auto largest_shape = std::max_element(
        shapes.begin(), shapes.end(),
        [](const std::unique_ptr<Shape>& lhs, const std::unique_ptr<Shape>& rhs) {
            return lhs->Area() < rhs->Area();
        });
    std::cout << "largest: " << (*largest_shape)->Name() << " ("
              << (*largest_shape)->Area() << ")\n";
    return 0;
}
