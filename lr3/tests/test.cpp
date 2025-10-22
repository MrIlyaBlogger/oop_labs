#include "gtest/gtest.h"
#include "../include/oop_lr3.hpp"
#include <sstream>

using namespace oop;

class FiguresTest : public ::testing::Test {
protected:
    std::stringstream ss;
};

TEST_F(FiguresTest, RectangleAreaCentroid) {
    Rectangle r({Point{0,0}, Point{2,0}, Point{2,1}, Point{0,1}});
    EXPECT_NEAR(r.area(), 2.0, 1e-9);
    auto c = r.centroid();
    EXPECT_NEAR(c.x, 1.0, 1e-9);
    EXPECT_NEAR(c.y, 0.5, 1e-9);
}

TEST_F(FiguresTest, SquareAreaCentroid) {
    Square s({Point{0,0}, Point{1,0}, Point{1,1}, Point{0,1}});
    EXPECT_NEAR(s.area(), 1.0, 1e-9);
    auto c = s.centroid();
    EXPECT_NEAR(c.x, 0.5, 1e-9);
    EXPECT_NEAR(c.y, 0.5, 1e-9);
}

TEST_F(FiguresTest, TrapezoidAreaCentroid) {
    Trapezoid t({Point{0,0}, Point{4,0}, Point{3,2}, Point{1,2}});
    EXPECT_NEAR(t.area(), 6.0, 1e-9);
    auto c = t.centroid();
    EXPECT_NEAR(c.x, 2.0, 1e-9);
    EXPECT_NEAR(c.y, 0.8888888888888888, 1e-9);
}

TEST_F(FiguresTest, StreamInputSquare) {
    ss.str("1 0 0 1 0 1 1 0 1");
    Figure* f = nullptr;
    ss >> f;
    ASSERT_TRUE(f != nullptr);
    EXPECT_NEAR(f->area(), 1.0, 1e-9);
    delete f;
}

TEST_F(FiguresTest, StreamInputRectangle) {
    ss.str("2 0 0 2 0 2 1 0 1");
    Figure* f = nullptr;
    ss >> f;
    ASSERT_TRUE(f != nullptr);
    EXPECT_NEAR(f->area(), 2.0, 1e-9);
    delete f;
}

TEST_F(FiguresTest, StreamInputTrapezoid) {
    ss.str("3 0 0 4 0 3 2 1 2");
    Figure* f = nullptr;
    ss >> f;
    ASSERT_TRUE(f != nullptr);
    EXPECT_NEAR(f->area(), 6.0, 1e-9);
    delete f;
}

TEST_F(FiguresTest, StreamInputInvalid) {
    ss.str("4 0 0 1 1 2 2 3 3");
    Figure* f = nullptr;
    ss >> f;
    EXPECT_TRUE(f == nullptr);
}

TEST_F(FiguresTest, CopyAndEquals) {
    Rectangle r1({Point{0,0}, Point{2,0}, Point{2,1}, Point{0,1}});
    auto r2 = r1;
    EXPECT_TRUE(r1.equals(r2));
    EXPECT_NEAR(r2.area(), 2.0, 1e-9);
}

TEST_F(FiguresTest, CloneAndEquals) {
    Square s1({Point{0,0}, Point{1,0}, Point{1,1}, Point{0,1}});
    std::unique_ptr<Figure> s2(s1.clone());
    EXPECT_TRUE(s1.equals(*s2));
    EXPECT_NEAR(s2->area(), 1.0, 1e-9);
}

TEST_F(FiguresTest, MoveConstructor) {
    Trapezoid t1({Point{0,0}, Point{4,0}, Point{3,2}, Point{1,2}});
    double area = t1.area();
    Trapezoid t2(std::move(t1));
    EXPECT_NEAR(t2.area(), area, 1e-9);
}

TEST_F(FiguresTest, ConversionToDouble) {
    Square s({Point{0,0}, Point{2,0}, Point{2,2}, Point{0,2}});
    EXPECT_NEAR(double(s), 4.0, 1e-9);
}
