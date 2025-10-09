#include <gtest/gtest.h>
#include "oop_lr2.hpp"

TEST(ElevenTest, DefaultConstructor) {
    Eleven a;
    EXPECT_EQ(a.length(), 0);
    EXPECT_EQ(a.toString(), "0");
}

TEST(ElevenTest, FromString) {
    Eleven a("123");
    EXPECT_EQ(a.toString(), "123");

    Eleven b("A");
    EXPECT_EQ(b.toString(), "A");

    EXPECT_THROW(Eleven("B"), std::invalid_argument);
}

TEST(ElevenTest, FromInitializerList) {
    Eleven a({1, 0, 10});
    EXPECT_EQ(a.toString(), "A01");
}

TEST(ElevenTest, Addition) {
    Eleven a("128");
    Eleven b("94");
    Eleven c = a.add(b);
    EXPECT_EQ(c.toString(), "211");
}

TEST(ElevenTest, Subtraction) {
    Eleven a("128");
    Eleven b("94");
    Eleven c = a.sub(b);
    EXPECT_EQ(c.toString(), "44");

    Eleven d("94");
    EXPECT_THROW(d.sub(a), std::underflow_error);
}

TEST(ElevenTest, CopyConstructor) {
    Eleven a("95");
    Eleven b = a.copy();
    EXPECT_EQ(b.toString(), "95");
    EXPECT_TRUE(a.equal(b));
}

TEST(ElevenTest, Comparisons) {
    Eleven a("128");
    Eleven b("94");

    EXPECT_TRUE(a.greater(b));
    EXPECT_FALSE(a.less(b));
    EXPECT_FALSE(a.equal(b));

    Eleven c("128");
    EXPECT_TRUE(a.equal(c));
    EXPECT_FALSE(a.greater(c));
    EXPECT_FALSE(a.less(c));
}

TEST(ElevenTest, Immutability) {
    Eleven a("128");
    Eleven b("94");
    Eleven c = a.add(b);
    EXPECT_EQ(a.toString(), "128");
    EXPECT_EQ(b.toString(), "94");
    EXPECT_EQ(c.toString(), "211");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
