#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula::functions::builtin;
using xl_formula::Context;
using xl_formula::Value;

TEST(RomanArabicTest, RomanBasic) {
    auto r = roman({Value(1987.0)}, Context{});
    ASSERT_TRUE(r.isText());
    EXPECT_EQ(r.asText(), "MCMLXXXVII");
}

TEST(RomanArabicTest, ArabicBasic) {
    auto a = arabic({Value("MCMLXXXVII")}, Context{});
    ASSERT_TRUE(a.isNumber());
    EXPECT_DOUBLE_EQ(a.asNumber(), 1987.0);
}
