#include "../include/oop_lr4.hpp"

namespace oop {

template <typename T>
Triangle<T>::Triangle(const std::vector<Point<T>>& pts) : points(pts) {
    if (points.size() != 3) {
        throw std::invalid_argument("Triangle: expected 3 points");
    }
}

template <typename T>
double Triangle<T>::area() const {
    return 0.5 * std::abs(points[0].x * (points[1].y - points[2].y) + points[1].x * (points[2].y - points[0].y) + points[2].x * (points[0].y - points[1].y));
}

template <typename T>
Point<T> Triangle<T>::center() const {
    return {static_cast<T>((points[0].x + points[1].x + points[2].x) / 3.0), static_cast<T>((points[0].y + points[1].y + points[2].y) / 3.0)};
}

template <typename T>
void Triangle<T>::print(std::ostream& os) const {
    os << "Triangle: ";
    for (auto& p : points) {
        os << "(" << p.x << ", " << p.y << ") ";
    }
}

template <typename T>
Square<T>::Square(const std::vector<Point<T>>& pts) : points(pts) {
    if (points.size() != 4) {
        throw std::invalid_argument("Square: expected 4 points");
    }
}

template <typename T>
double Square<T>::area() const {
    double a = std::hypot(points[1].x - points[0].x, points[1].y - points[0].y);
    return a * a;
}

template <typename T>
Point<T> Square<T>::center() const {
    double cx = 0, cy = 0;
    for (auto& p : points) {
        cx += p.x;
        cy += p.y;
    }
    return {static_cast<T>(cx / 4.0), static_cast<T>(cy / 4.0)};
}

template <typename T>
void Square<T>::print(std::ostream& os) const {
    os << "Square: ";
    for (auto& p : points) {
        os << "(" << p.x << ", " << p.y << ") ";
    }
}

template <typename T>
Octagon<T>::Octagon(const std::vector<Point<T>>& pts) : points(pts) {
    if (points.size() != 8) {
        throw std::invalid_argument("Octagon: expected 8 points");
    }
}

template <typename T>
double Octagon<T>::area() const {
    double sum = 0.0;
    for (size_t i = 0; i < 8; ++i) {
        size_t j = (i + 1) % 8;
        sum += points[i].x * points[j].y - points[j].x * points[i].y;
    }
    return std::abs(sum) / 2.0;
}

template <typename T>
Point<T> Octagon<T>::center() const {
    double cx = 0, cy = 0;
    for (auto& p : points) {
        cx += p.x;
        cy += p.y;
    }
    return {static_cast<T>(cx / 8.0), static_cast<T>(cy / 8.0)};
}

template <typename T>
void Octagon<T>::print(std::ostream& os) const {
    os << "Octagon: ";
    for (auto& p : points) {
        os << "(" << p.x << ", " << p.y << ") ";
    }
}

template <typename T>
void Array<T>::add(std::unique_ptr<Figure<T>> f) {
    data.push_back(std::move(f));
}

template <typename T>
void Array<T>::print(std::ostream& os) const {
    for (auto& f : data) {
        f->print(os);
        os << " | Area = " << f->area()
           << " | Center = (" << f->center().x << ", " << f->center().y << ")\n";
    }
}

template <typename T>
double Array<T>::total_area() const {
    double sum = 0;
    for (auto& f : data) {
        sum += f->area();
    }
    return sum;
}

std::istream& operator>>(std::istream& in, std::unique_ptr<FigureD>& fptr) {
    int type;
    if (!(in >> type)) {
        return in;
    }

    if (type == 1) {
        std::vector<PointD> pts(3);
        for (auto& p : pts) {
            in >> p.x >> p.y;
        }
        fptr = std::make_unique<TriangleD>(pts);
    } else if (type == 2) {
        std::vector<PointD> pts(4);
        for (auto& p : pts) {
            in >> p.x >> p.y;
        }
        fptr = std::make_unique<SquareD>(pts);
    } else if (type == 3) {
        std::vector<PointD> pts(8);
        for (auto& p : pts) {
            in >> p.x >> p.y;
        }
        fptr = std::make_unique<OctagonD>(pts);
    } else {
        fptr.reset();
    }
    return in;
}

template class Triangle<int>;
template class Triangle<double>;
template class Square<int>;
template class Square<double>;
template class Octagon<int>;
template class Octagon<double>;
template class Array<double>;

}
