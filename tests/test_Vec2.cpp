#include <gtest/gtest.h>
#include "Vec2.hpp"

TEST(Vec2Test, DefaultConstructor) {
    Vec2 v;
    EXPECT_EQ(v.x, 0);
    EXPECT_EQ(v.y, 0);
}

TEST(Vec2Test, ParameterizedConstructor) {
    Vec2 v(1.0f, 2.0f);
    EXPECT_EQ(v.x, 1.0f);
    EXPECT_EQ(v.y, 2.0f);
}

TEST(Vec2Test, EqualityOperator) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(1.0f, 2.0f);
    EXPECT_TRUE(v1 == v2);
}

TEST(Vec2Test, InequalityOperator) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(3.0f, 4.0f);
    EXPECT_TRUE(v1 != v2);
}

TEST(Vec2Test, AdditionOperator) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(3.0f, 4.0f);
    Vec2 v3 = v1 + v2;
    EXPECT_EQ(v3.x, 4.0f);
    EXPECT_EQ(v3.y, 6.0f);
}

TEST(Vec2Test, SubtractionOperator) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(3.0f, 4.0f);
    Vec2 v3 = v2 - v1;
    EXPECT_EQ(v3.x, 2.0f);
    EXPECT_EQ(v3.y, 2.0f);
}

TEST(Vec2Test, MultiplicationOperator) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2 = v1 * 2.0f;
    EXPECT_EQ(v2.x, 2.0f);
    EXPECT_EQ(v2.y, 4.0f);
}

TEST(Vec2Test, DivisionOperator) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2 = v1 / 2.0f;
    EXPECT_EQ(v2.x, 0.5f);
    EXPECT_EQ(v2.y, 1.0f);
}

TEST(Vec2Test, AdditionAssignmentOperator) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(3.0f, 4.0f);
    v1 += v2;
    EXPECT_EQ(v1.x, 4.0f);
    EXPECT_EQ(v1.y, 6.0f);
}

TEST(Vec2Test, SubtractionAssignmentOperator) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(3.0f, 4.0f);
    v2 -= v1;
    EXPECT_EQ(v2.x, 2.0f);
    EXPECT_EQ(v2.y, 2.0f);
}

TEST(Vec2Test, MultiplicationAssignmentOperator) {
    Vec2 v1(1.0f, 2.0f);
    v1 *= 2.0f;
    EXPECT_EQ(v1.x, 2.0f);
    EXPECT_EQ(v1.y, 4.0f);
}

TEST(Vec2Test, DivisionAssignmentOperator) {
    Vec2 v1(1.0f, 2.0f);
    v1 /= 2.0f;
    EXPECT_EQ(v1.x, 0.5f);
    EXPECT_EQ(v1.y, 1.0f);
}

TEST(Vec2Test, Normalize) {
    Vec2 v(1.0f, 2.0f);
    v.normalize();
    EXPECT_FLOAT_EQ(v.x, 0.44721359f);
    EXPECT_FLOAT_EQ(v.y, 0.89442718f);

    Vec2 v2(3.0f, 4.0f);
    v.normalize();
    EXPECT_FLOAT_EQ(v2.x, 0.6f);
    EXPECT_FLOAT_EQ(v2.y, 0.8f);
}

TEST(Vec2Test, Length) {
    Vec2 v(1.0f, 2.0f);
    EXPECT_FLOAT_EQ(v.length(), 2.23606798f);

    Vec2 v2(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v2.length(), 5.0f);
}

TEST(Vec2Test, Distance) {
    Vec2 v1(1.0f, 3.0f);
    Vec2 v2(2.0f, 4.0f);
    EXPECT_FLOAT_EQ(v1.distance(v2), 1.41421356f);
}
// Add more tests for the remaining operators and member functions