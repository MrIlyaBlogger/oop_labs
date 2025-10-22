#include "gtest/gtest.h"
#include "../include/oop_lr3.hpp"

using namespace oop;

TEST(Figures, RectangleAreaCentroid) {
    Rectangle r({Point{0,0}, Point{2,0}, Point{2,1}, Point{0,1}});
    EXPECT_NEAR(r.area(), 2.0, 1e-9);
    auto c = r.centroid();
    EXPECT_NEAR(c.x, 1.0, 1e-9);
    EXPECT_NEAR(c.y, 0.5, 1e-9);
}

TEST(Figures, SquareAreaCentroid) {
    Square s({Point{0,0}, Point{1,0}, Point{1,1}, Point{0,1}});
    EXPECT_NEAR(s.area(), 1.0, 1e-9);
    auto c = s.centroid();
    EXPECT_NEAR(c.x, 0.5, 1e-9);
    EXPECT_NEAR(c.y, 0.5, 1e-9);
}

TEST(Figures, TrapezoidAreaCentroid) {
    Trapezoid t({Point{0,0}, Point{4,0}, Point{3,2}, Point{1,2}});
    EXPECT_NEAR(t.area(), 6.0, 1e-9);
}
