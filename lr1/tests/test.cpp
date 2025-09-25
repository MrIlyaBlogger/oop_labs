#include <gtest/gtest.h>
#include "../oop_lr1.cpp"  // подключаем cpp напрямую

TEST(BracketTest, EmptyString) {
    EXPECT_TRUE(isValid(""));
}

TEST(BracketTest, ValidStrings) {
    EXPECT_TRUE(isValid("()"));
    EXPECT_TRUE(isValid("(())((()())())"));
}

TEST(BracketTest, InvalidStrings) {
    EXPECT_FALSE(isValid(")("));
    EXPECT_FALSE(isValid("(()))"));
    EXPECT_FALSE(isValid("("));
}

TEST(BracketTest, MoreCases) {
    EXPECT_TRUE(isValid("()()()"));
    EXPECT_FALSE(isValid("((())"));
}

// main для Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
