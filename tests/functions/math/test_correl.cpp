#include <gtest/gtest.h>
#include "velox/formulas/functions.h"

using namespace xl_formula::functions::builtin;
using xl_formula::Context;
using xl_formula::Value;

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
    auto res = correl({Value(1.0), Value(2.0), Value(3.0), Value(2.0), Value(4.0), Value(6.0)},
                      Context{});
    ASSERT_TRUE(res.isNumber());
    EXPECT_NEAR(res.asNumber(), 1.0, 1e-9);
}

TEST(CorrelFunctionTest, PearsonAliasAndRSQ) {
    auto p = correl({Value(std::vector<Value>{Value(1.0), Value(2.0), Value(3.0)}),
                     Value(std::vector<Value>{Value(2.0), Value(4.0), Value(6.0)})},
                    Context{});
    ASSERT_TRUE(p.isNumber());
    EXPECT_DOUBLE_EQ(p.asNumber(), 1.0);
    auto r2 = rsq({Value(std::vector<Value>{Value(1.0), Value(2.0), Value(3.0)}),
                   Value(std::vector<Value>{Value(2.0), Value(4.0), Value(6.0)})},
                  Context{});
    ASSERT_TRUE(r2.isNumber());
    EXPECT_DOUBLE_EQ(r2.asNumber(), 1.0);
}

TEST(CorrelFunctionTest, SlopeAndIntercept) {
    auto m = slope({Value(std::vector<Value>{Value(2.0), Value(4.0), Value(6.0), Value(8.0)}),
                    Value(std::vector<Value>{Value(1.0), Value(2.0), Value(3.0), Value(4.0)})},
                   Context{});
    ASSERT_TRUE(m.isNumber());
    EXPECT_DOUBLE_EQ(m.asNumber(), 2.0);
    auto b = intercept({Value(std::vector<Value>{Value(2.0), Value(4.0), Value(6.0), Value(8.0)}),
                        Value(std::vector<Value>{Value(1.0), Value(2.0), Value(3.0), Value(4.0)})},
                       Context{});
    ASSERT_TRUE(b.isNumber());
    EXPECT_NEAR(b.asNumber(), 0.0, 1e-12);
}
