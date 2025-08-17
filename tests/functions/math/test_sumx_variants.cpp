#include <gtest/gtest.h>
#include "velox/formulas/functions.h"

using namespace xl_formula::functions::builtin;
using xl_formula::Context;
using xl_formula::Value;

TEST(SumXVariantsTest, SumX2MY2_ArrayInputs) {
    std::vector<Value> a = {Value(1.0), Value(2.0)};
    std::vector<Value> b = {Value(3.0), Value(4.0)};
    auto res = sumx2my2({Value(a), Value(b)}, Context{});
    ASSERT_TRUE(res.isNumber());
    EXPECT_DOUBLE_EQ(res.asNumber(), (1 * 1 - 3 * 3) + (2 * 2 - 4 * 4));
}

TEST(SumXVariantsTest, SumX2PY2_Flat) {
    auto res = sumx2py2({Value(1.0), Value(2.0), Value(3.0), Value(4.0)}, Context{});
    ASSERT_TRUE(res.isNumber());
    EXPECT_DOUBLE_EQ(res.asNumber(), (1 * 1 + 2 * 2) + (3 * 3 + 4 * 4));
}

TEST(SumXVariantsTest, SumXMY2_Flat) {
    auto res = sumxmy2({Value(5.0), Value(7.0), Value(2.0), Value(4.0)}, Context{});
    ASSERT_TRUE(res.isNumber());
    EXPECT_DOUBLE_EQ(res.asNumber(), (5 - 2) * (5 - 2) + (7 - 4) * (7 - 4));
}
