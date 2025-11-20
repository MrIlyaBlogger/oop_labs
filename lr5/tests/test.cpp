#include "gtest/gtest.h"

#include "../include/oop_lr5.hpp"

#include <string>

namespace {

oop::DemoComplexType make_complex(std::string name, int id, double value) {
    return oop::DemoComplexType{std::move(name), id, value};
}

}  // namespace

TEST(PmrStackTest, PushPopIntegers) {
    oop::VectorTrackingMemoryResource resource;
    oop::PmrStack<int> stack(&resource);

    stack.push(1);
    stack.push(2);
    stack.push(3);

    EXPECT_EQ(stack.size(), 3u);
    EXPECT_EQ(stack.top(), 3);

    stack.pop();
    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.size(), 2u);
}

TEST(PmrStackTest, HandlesComplexTypes) {
    oop::VectorTrackingMemoryResource resource;
    oop::PmrStack<oop::DemoComplexType> stack(&resource);

    stack.push(make_complex("alpha", 1, 3.14));
    stack.emplace(oop::DemoComplexType{"beta", 2, 2.71});
    stack.emplace(oop::DemoComplexType{"gamma", 3, 1.41});

    EXPECT_EQ(stack.top().name, "gamma");
    stack.pop();

    auto top = stack.top();
    EXPECT_EQ(top.name, "beta");
    EXPECT_EQ(top.id, 2);
    EXPECT_DOUBLE_EQ(top.value, 2.71);
}

TEST(PmrStackTest, ReusesReleasedBlocks) {
    oop::VectorTrackingMemoryResource resource;
    oop::PmrStack<int> stack(&resource);

    stack.push(10);
    stack.push(20);
    const auto tracked_after_push = resource.tracked_blocks();
    EXPECT_EQ(tracked_after_push, 2u);

    stack.pop();
    stack.pop();
    EXPECT_EQ(resource.reusable_blocks(), tracked_after_push);

    stack.push(30);
    EXPECT_EQ(resource.tracked_blocks(), tracked_after_push);
    EXPECT_EQ(resource.active_blocks(), 1u);
}
