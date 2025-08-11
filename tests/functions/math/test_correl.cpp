#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula::functions::builtin;
using xl_formula::Value;
using xl_formula::Context;

TEST(CorrelFunctionTest, SimplePositiveCorrelation) {
    std::vector<Value> a = {Value(1.0), Value(2.0), Value(3.0), Value(4.0)};
    std::vector<Value> b = {Value(2.0), Value(4.0), Value(6.0), Value(8.0)};
    std::vector<Value> args = {Value(a), Value(b)};
    auto res = correl(args, Context{});
    ASSERT_TRUE(res.isNumber());
    EXPECT_NEAR(res.asNumber(), 1.0, 1e-9);
}

TEST(CorrelFunctionTest, NegativeCorrelation) {
    std::vector<Value> a = {Value(1.0), Value(2.0), Value(3.0)};
    std::vector<Value> b = {Value(3.0), Value(2.0), Value(1.0)};
    auto res = correl({Value(a), Value(b)}, Context{});
    ASSERT_TRUE(res.isNumber());
    EXPECT_NEAR(res.asNumber(), -1.0, 1e-9);
}

TEST(CorrelFunctionTest, FlatListSplit) {
    auto res = correl({Value(1.0), Value(2.0), Value(3.0), Value(2.0), Value(4.0), Value(6.0)}, Context{});
    ASSERT_TRUE(res.isNumber());
    EXPECT_NEAR(res.asNumber(), 1.0, 1e-9);
}


