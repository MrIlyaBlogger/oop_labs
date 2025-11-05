#include "gtest/gtest.h"
#include "../include/oop_lr4.hpp"
#include <sstream>

using namespace oop;

class FiguresTest : public ::testing::Test {
protected:
    std::stringstream ss;
};

TEST_F(FiguresTest, TriangleAreaAndCenter) {
    std::vector<PointD> pts = {{0, 0}, {4, 0}, {0, 3}};
    TriangleD t(pts);
    EXPECT_NEAR(t.area(), 6.0, 1e-9);
    auto c = t.center();
    EXPECT_NEAR(c.x, 1.3333333333, 1e-6);
    EXPECT_NEAR(c.y, 1.0, 1e-6);
}

TEST_F(FiguresTest, SquareAreaAndCenter) {
    std::vector<PointD> pts = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    SquareD s(pts);
    EXPECT_NEAR(s.area(), 4.0, 1e-9);
    auto c = s.center();
    EXPECT_NEAR(c.x, 1.0, 1e-9);
    EXPECT_NEAR(c.y, 1.0, 1e-9);
}

TEST_F(FiguresTest, OctagonAreaAndCenter) {
    std::vector<PointD> pts = {
        {1,0}, {2,0}, {3,1}, {3,2}, {2,3}, {1,3}, {0,2}, {0,1}
    };
    OctagonD o(pts);
    EXPECT_GT(o.area(), 0.0);
    auto c = o.center();
    EXPECT_NEAR(c.x, 1.5, 1e-6);
    EXPECT_NEAR(c.y, 1.5, 1e-6);
}

TEST_F(FiguresTest, StreamInputTriangle) {
    ss.str("1 0 0 4 0 0 3");
    std::unique_ptr<FigureD> f = nullptr;
    ss >> f;
    ASSERT_TRUE(f != nullptr);
    EXPECT_NEAR(f->area(), 6.0, 1e-9);
}

TEST_F(FiguresTest, StreamInputSquare) {
    ss.str("2 0 0 2 0 2 2 0 2");
    std::unique_ptr<FigureD> f = nullptr;
    ss >> f;
    ASSERT_TRUE(f != nullptr);
    EXPECT_NEAR(f->area(), 4.0, 1e-9);
}

TEST_F(FiguresTest, StreamInputOctagon) {
    ss.str("3 1 0 2 0 3 1 3 2 2 3 1 3 0 2 0 1");
    std::unique_ptr<FigureD> f = nullptr;
    ss >> f;
    ASSERT_TRUE(f != nullptr);
    EXPECT_GT(f->area(), 0.0);
}

TEST_F(FiguresTest, StreamInputInvalid) {
    ss.str("4 0 0 1 1 2 2 3 3");
    std::unique_ptr<FigureD> f = nullptr;
    ss >> f;
    EXPECT_TRUE(f == nullptr);
}

TEST_F(FiguresTest, MoveConstructor) {
    TriangleD t1({{0, 0}, {4, 0}, {0, 3}});
    double area = t1.area();
    TriangleD t2(std::move(t1));
    EXPECT_NEAR(t2.area(), area, 1e-9);
}