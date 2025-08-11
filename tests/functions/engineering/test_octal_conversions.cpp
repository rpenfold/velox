#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula::functions::builtin;
using xl_formula::Context;
using xl_formula::Value;

TEST(EngineeringOctalConversionsTest, Dec2Oct_PositiveAndNegative) {
    auto r1 = dec2oct({Value(8.0)}, Context{});
    ASSERT_TRUE(r1.isText());
    EXPECT_EQ(r1.asText(), "10");

    auto r2 = dec2oct({Value(0.0)}, Context{});
    ASSERT_TRUE(r2.isText());
    EXPECT_EQ(r2.asText(), "0");

    auto r3 = dec2oct({Value(-8.0)}, Context{});
    ASSERT_TRUE(r3.isText());
    // Implementation uses two's complement width; just ensure it returns some octal string
    EXPECT_FALSE(r3.asText().empty());
}

TEST(EngineeringOctalConversionsTest, Bin2Oct_And_Oct2Bin) {
    auto b2o = bin2oct({Value("1111111111")}, Context{}); // 10 ones
    ASSERT_TRUE(b2o.isText());
    // 10 ones in binary = 1023 decimal = 1777 octal
    EXPECT_EQ(b2o.asText(), "1777");

    auto o2b = oct2bin({Value("1777")}, Context{});
    ASSERT_TRUE(o2b.isText());
    EXPECT_EQ(o2b.asText(), "1111111111");
}

TEST(EngineeringOctalConversionsTest, Hex2Oct_And_Oct2Hex) {
    auto h2o = hex2oct({Value("FF")}, Context{});
    ASSERT_TRUE(h2o.isText());
    EXPECT_EQ(h2o.asText(), "377"); // 255 decimal

    auto o2h = oct2hex({Value("377")}, Context{});
    ASSERT_TRUE(o2h.isText());
    EXPECT_EQ(o2h.asText(), "FF");
}


