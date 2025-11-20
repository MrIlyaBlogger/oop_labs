#include "../include/oop_lr5.hpp"
#include <gtest/gtest.h>
#include <string>

using namespace oop;

TEST(StackPmr, BasicPushPop) {
    VectorHeapMemoryResource mem;
    pmr_stack<int> st(&mem);

    st.push(10);
    st.push(20);

    EXPECT_EQ(st.size(), 2);
    EXPECT_EQ(st.top(), 20);

    st.pop();
    EXPECT_EQ(st.top(), 10);
}

TEST(StackPmr, EmplaceAndIteration) {
    VectorHeapMemoryResource mem;
    pmr_stack<std::string> st(&mem);

    st.emplace("hello");
    st.emplace("world");

    EXPECT_EQ(st.size(), 2);

    std::vector<std::string> out;
    for (auto& v : st) out.push_back(v);

    EXPECT_EQ(out[0], "world");
    EXPECT_EQ(out[1], "hello");
}

TEST(StackPmr, MoveStack) {
    VectorHeapMemoryResource mem;
    pmr_stack<int> st(&mem);

    st.push(1);
    st.push(2);
    st.push(3);

    pmr_stack<int> mv = std::move(st);

    EXPECT_TRUE(st.empty());
    EXPECT_EQ(mv.size(), 3);
}
