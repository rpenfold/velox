#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula::functions::builtin;
using xl_formula::Context;
using xl_formula::Value;

TEST(EngineeringComplexTest, Complex_Construct_And_Extract) {
    auto c = complex_function({Value(3.0), Value(-4.0)}, Context{});
    ASSERT_TRUE(c.isText());
    EXPECT_EQ(c.asText(), "3-4i");

    auto r = imreal({c}, Context{});
    ASSERT_TRUE(r.isNumber());
    EXPECT_DOUBLE_EQ(r.asNumber(), 3.0);

    auto im = imaginary({c}, Context{});
    ASSERT_TRUE(im.isNumber());
    EXPECT_DOUBLE_EQ(im.asNumber(), -4.0);
}

TEST(EngineeringComplexTest, Parse_Imaginary_Forms) {
    auto r1 = imreal({Value("5i")}, Context{});
    EXPECT_DOUBLE_EQ(r1.asNumber(), 0.0);
    auto im1 = imaginary({Value("5i")}, Context{});
    EXPECT_DOUBLE_EQ(im1.asNumber(), 5.0);

    auto r2 = imreal({Value("-2+3i")}, Context{});
    EXPECT_DOUBLE_EQ(r2.asNumber(), -2.0);
    auto im2 = imaginary({Value("-2+3i")}, Context{});
    EXPECT_DOUBLE_EQ(im2.asNumber(), 3.0);
}


