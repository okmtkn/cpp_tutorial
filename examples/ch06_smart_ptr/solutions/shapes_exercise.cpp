// 第6章 演習1〜3: Triangle 追加・最大面積の図形・周長 perimeter
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

constexpr double PI = 3.14159265358979323846;

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;  // 演習3
    virtual const char* name() const = 0;
};

class Circle : public Shape {
public:
    Circle(double r) : r_(r) {}
    double area() const override { return PI * r_ * r_; }
    double perimeter() const override { return 2.0 * PI * r_; }
    const char* name() const override { return "Circle"; }

private:
    double r_;
};

class Rect : public Shape {
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    double perimeter() const override { return 2.0 * (w_ + h_); }
    const char* name() const override { return "Rect"; }

private:
    double w_, h_;
};

// 演習1: 三角形（周長計算のため直角三角形とし、底辺と高さから3辺を出す）
class Triangle : public Shape {
public:
    Triangle(double base, double height) : b_(base), h_(height) {}
    double area() const override { return 0.5 * b_ * h_; }
    double perimeter() const override {
        return b_ + h_ + std::sqrt(b_ * b_ + h_ * h_);
    }
    const char* name() const override { return "Triangle"; }

private:
    double b_, h_;
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rect>(2.0, 3.0));
    shapes.push_back(std::make_unique<Triangle>(3.0, 4.0));

    for (const auto& s : shapes) {
        std::cout << s->name() << ": area = " << s->area()
                  << ", perimeter = " << s->perimeter() << "\n";
    }

    // 演習2: 面積が最大の図形を max_element + ラムダで探す
    auto biggest = std::max_element(
        shapes.begin(), shapes.end(),
        [](const std::unique_ptr<Shape>& a, const std::unique_ptr<Shape>& b) {
            return a->area() < b->area();
        });
    std::cout << "biggest: " << (*biggest)->name() << " ("
              << (*biggest)->area() << ")\n";
    return 0;
}
