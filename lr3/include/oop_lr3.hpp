#ifndef OOP_LR3_HPP
#define OOP_LR3_HPP

#include <array>
#include <iostream>
#include <vector>
#include <cmath>

namespace oop {

struct Point {
    double x{0.0};
    double y{0.0};
};

inline bool almost_eq(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}

class Figure {
public:
    virtual ~Figure() = default;

    virtual Point centroid() const = 0;

    virtual void print(std::ostream &os) const = 0;

    virtual void read(std::istream &is) = 0;

    virtual double area() const = 0;

    virtual Figure *clone() const = 0;

    explicit operator double() const { 
        return area(); 
    }

    virtual bool equals(const Figure &other) const = 0;
};

class Quadrilateral : public Figure {
public:
    Quadrilateral() = default;
    Quadrilateral(const std::array<Point,4> &verts) : v(verts) {}

    Quadrilateral(const Quadrilateral &other) = default;
    Quadrilateral(Quadrilateral &&other) noexcept = default;
    Quadrilateral &operator=(const Quadrilateral &other) = default;
    Quadrilateral &operator=(Quadrilateral &&other) noexcept = default;

    Point centroid() const override;
    void print(std::ostream &os) const override;
    double area() const override;
    bool equals(const Figure &other) const override;

protected:
    std::array<Point,4> v{};
};

class Square : public Quadrilateral {
public:
    Square() = default;
    Square(const std::array<Point,4> &verts) : Quadrilateral(verts) {}

    Square(const Square &other) = default;
    Square(Square &&other) noexcept = default;
    Square &operator=(const Square &other) = default;
    Square &operator=(Square &&other) noexcept = default;

    void read(std::istream &is) override;
    Figure *clone() const override { 
        return new Square(*this); 
    }
};

class Rectangle : public Quadrilateral {
public:
    Rectangle() = default;
    Rectangle(const std::array<Point,4> &verts) : Quadrilateral(verts) {}

    Rectangle(const Rectangle &other) = default;
    Rectangle(Rectangle &&other) noexcept = default;
    Rectangle &operator=(const Rectangle &other) = default;
    Rectangle &operator=(Rectangle &&other) noexcept = default;

    void read(std::istream &is) override;
    Figure *clone() const override { 
        return new Rectangle(*this); 
    }
};

class Trapezoid : public Quadrilateral {
public:
    Trapezoid() = default;
    Trapezoid(const std::array<Point,4> &verts) : Quadrilateral(verts) {}

    Trapezoid(const Trapezoid &other) = default;
    Trapezoid(Trapezoid &&other) noexcept = default;
    Trapezoid &operator=(const Trapezoid &other) = default;
    Trapezoid &operator=(Trapezoid &&other) noexcept = default;

    void read(std::istream &is) override;
    Figure *clone() const override { 
        return new Trapezoid(*this); 
    }
};

inline std::ostream &operator<<(std::ostream &os, const Figure &f) {
    f.print(os);
    return os;
}


}

#endif
