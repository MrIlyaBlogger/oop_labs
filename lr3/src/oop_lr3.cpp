#include "../include/oop_lr3.hpp"
#include <iostream>

namespace oop {

static double polygon_area_signed(const std::vector<Point> &pts) {
    double a = 0.0;
    size_t n = pts.size();
    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        a += pts[i].x * pts[j].y - pts[j].x * pts[i].y;
    }
    return a / 2.0;
}

static Point polygon_centroid(const std::vector<Point> &pts) {
    Point c{0.0, 0.0};
    double a = polygon_area_signed(pts);
    if (almost_eq(a, 0.0)) return c;
    double cx = 0.0;
    double cy = 0.0;
    size_t n = pts.size();
    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        double cross = pts[i].x * pts[j].y - pts[j].x * pts[i].y;
        cx += (pts[i].x + pts[j].x) * cross;
        cy += (pts[i].y + pts[j].y) * cross;
    }
    cx /= (6.0 * a);
    cy /= (6.0 * a);
    c.x = cx;
    c.y = cy;
    return c;
}

Point Quadrilateral::centroid() const {
    std::vector<Point> pts(v.begin(), v.end());
    return polygon_centroid(pts);
}

void Quadrilateral::print(std::ostream &os) const {
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << "(" << v[i].x << "," << v[i].y << ")";
        if (i + 1 < v.size()) os << ", ";
    }
    os << "]";
}

double Quadrilateral::area() const {
    std::vector<Point> pts(v.begin(), v.end());
    return std::fabs(polygon_area_signed(pts));
}

bool Quadrilateral::equals(const Figure &other) const {
    auto p = dynamic_cast<const Quadrilateral *>(&other);
    if (!p) return false;
    for (size_t i = 0; i < 4; ++i) {
        if (!almost_eq(v[i].x, p->v[i].x) || !almost_eq(v[i].y, p->v[i].y)) return false;
    }
    return true;
}

void Square::read(std::istream &is) {

    for (int i = 0; i < 4; ++i) {
        is >> v[i].x >> v[i].y;
    }
}

void Rectangle::read(std::istream &is) {
    for (int i = 0; i < 4; ++i) {
        is >> v[i].x >> v[i].y;
    }
}

void Trapezoid::read(std::istream &is) {
    for (int i = 0; i < 4; ++i) {
        is >> v[i].x >> v[i].y;
    }
}

}
