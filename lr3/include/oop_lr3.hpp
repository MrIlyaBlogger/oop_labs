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

    // Centroid of the polygon
    virtual Point centroid() const = 0;

    // Print vertices to ostream
    virtual void print(std::ostream &os) const = 0;

    // Read vertices from istream
    virtual void read(std::istream &is) = 0;

    // Area of polygon
    virtual double area() const = 0;

    // Polymorphic copy
    virtual Figure *clone() const = 0;

    // conversion to double -> area
    explicit operator double() const { return area(); }

    // equality
    virtual bool equals(const Figure &other) const = 0;
};

// Quadrilateral helper base for 4-vertex figures
class Quadrilateral : public Figure {
public:
    Quadrilateral() = default;
    Quadrilateral(const std::array<Point,4> &verts) : v(verts) {}

    // copy and move
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

    // copy/move override to satisfy requirement
    Square(const Square &other) = default;
    Square(Square &&other) noexcept = default;
    Square &operator=(const Square &other) = default;
    Square &operator=(Square &&other) noexcept = default;

    void read(std::istream &is) override;
    Figure *clone() const override { return new Square(*this); }
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
    Figure *clone() const override { return new Rectangle(*this); }
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
    Figure *clone() const override { return new Trapezoid(*this); }
};

// Non-member stream operators
inline std::ostream &operator<<(std::ostream &os, const Figure &f) {
    f.print(os);
    return os;
}

// operator>> will be provided per-type in the cpp file as free functions

} // namespace oop

#endif // OOP_LR3_HPP
