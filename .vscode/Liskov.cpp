#include <iostream>
#include <vector>

// Abstract class representing a shape
class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
};

// Concrete class representing a rectangle
class Rectangle : public Shape {
public:
    Rectangle(double length, double width) : length_(length), width_(width) {}

    double area() const override {
        return length_ * width_;
    }

    double perimeter() const override {
        return 2 * (length_ + width_);
    }

private:
    double length_;
    double width_;
};

// Concrete class representing a square
class Square : public Shape {
public:
    Square(double sideLength) : sideLength_(sideLength) {}

    double area() const override {
        return sideLength_ * sideLength_;
    }

    double perimeter() const override {
        return 4 * sideLength_;
    }

private:
    double sideLength_;
};

// Function that calculates the total area of a vector of shapes
double totalArea(const std::vector<Shape*>& shapes) {
    double area = 0;
    for (const auto& shape : shapes) {
        area += shape->area();
    }
    return area;
}

int main() {
    //Liskov: trong code, minh khai bao cai shape, tu do ngay khi minh declare shape, minh co the tinh toan dien tich vaf chu vi.
    //Code chi depend vao class Shape (abstract class) ma khong phu thuoc vao implementation cu the.
    Rectangle rectangle(3, 4);
    Square square(5);

    std::vector<Shape*> shapes;
    shapes.push_back(&rectangle);
    shapes.push_back(&square);


    double total_area = 0.0;
    for (const auto& shape : shapes) {
        total_area += shape->area();
    }

    std::cout << "Total area: " << total_area << std::endl;

    return 0;
}


