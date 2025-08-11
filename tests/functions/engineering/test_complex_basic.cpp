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

TEST(EngineeringComplexTest, Complex_MoreOps) {
    auto c = Value("3-4i");
    auto mag = imabs({c}, Context{});
    ASSERT_TRUE(mag.isNumber());
    EXPECT_DOUBLE_EQ(mag.asNumber(), 5.0);
    auto arg = imargument({c}, Context{});
    ASSERT_TRUE(arg.isNumber());
    EXPECT_NEAR(arg.asNumber(), std::atan2(-4.0, 3.0), 1e-12);

    auto s = imsum({Value("1+2i"), Value("3+4i")}, Context{});
    ASSERT_TRUE(s.isText());
    EXPECT_EQ(s.asText(), "4+6i");
    auto sub = imsub({Value("3+4i"), Value("1+2i")}, Context{});
    ASSERT_TRUE(sub.isText());
    EXPECT_EQ(sub.asText(), "2+2i");

    auto prod = improduct({Value("1+2i"), Value("3+4i")}, Context{});
    ASSERT_TRUE(prod.isText());
    EXPECT_EQ(prod.asText(), "-5+10i");
    auto div = imdiv({Value("3+4i"), Value("1+2i")}, Context{});
    ASSERT_TRUE(div.isText());
    EXPECT_EQ(div.asText(), "2.2-0.4i");

    auto p2 = impower({Value("1+2i"), Value(2.0)}, Context{});
    ASSERT_TRUE(p2.isText());
    EXPECT_EQ(p2.asText(), "-3+4i");
}
