#ifndef OOP_LR4_HPP
#define OOP_LR4_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

namespace oop {

template <typename T>
struct Point {
    T x{};
    T y{};
};

template <typename T>
class Figure {
public:
    virtual double area() const = 0;
    virtual Point<T> center() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual ~Figure() = default;
};

template <typename T>
class Triangle : public Figure<T> {
public:
    explicit Triangle(const std::vector<Point<T>>& pts);
    double area() const override;
    Point<T> center() const override;
    void print(std::ostream& os) const override;
private:
    std::vector<Point<T>> points;
};

template <typename T>
class Square : public Figure<T> {
public:
    explicit Square(const std::vector<Point<T>>& pts);
    double area() const override;
    Point<T> center() const override;
    void print(std::ostream& os) const override;
private:
    std::vector<Point<T>> points;
};

template <typename T>
class Octagon : public Figure<T> {
public:
    explicit Octagon(const std::vector<Point<T>>& pts);
    double area() const override;
    Point<T> center() const override;
    void print(std::ostream& os) const override;
private:
    std::vector<Point<T>> points;
};

template <typename T>
class Array {
public:
    void add(std::unique_ptr<Figure<T>> f);
    void print(std::ostream& os) const;
    double total_area() const;
private:
    std::vector<std::unique_ptr<Figure<T>>> data;
};

using PointD = Point<double>;
using FigureD = Figure<double>;
using TriangleD = Triangle<double>;
using SquareD = Square<double>;
using OctagonD = Octagon<double>;

std::istream& operator>>(std::istream& in, std::unique_ptr<FigureD>& fptr);

}

#endif
