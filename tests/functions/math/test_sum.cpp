#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class SumFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callSum(const std::vector<Value>& args) {
        return builtin::sum(args, context);
    }
};

TEST_F(SumFunctionTest, EmptyArguments_ReturnsZero) {
    auto result = callSum({});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(SumFunctionTest, SingleNumber_ReturnsSameNumber) {
    auto result = callSum({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(SumFunctionTest, MultipleNumbers_ReturnsSum) {
    auto result = callSum({Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(15.0, result.asNumber());
}

TEST_F(SumFunctionTest, MixedNumericTypes_ConvertsBooleans) {
    auto result = callSum({Value(1.0), Value(true), Value(false), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());  // 1 + 1 + 0 + 4
}

TEST_F(SumFunctionTest, WithTextValues_IgnoresText) {
    auto result = callSum({Value(1.0), Value("hello"), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());  // 1 + 5 (text ignored)
}

TEST_F(SumFunctionTest, WithEmptyValues_IgnoresEmpty) {
    auto result = callSum({Value(1.0), Value::empty(), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());  // 1 + 5 (empty ignored)
}

TEST_F(SumFunctionTest, WithErrorValue_PropagatesError) {
    auto result = callSum({Value(1.0), Value::error(ErrorType::DIV_ZERO), Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(SumFunctionTest, NegativeNumbers_HandlesCorrectly) {
    auto result = callSum({Value(10.0), Value(-3.0), Value(-2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(SumFunctionTest, DecimalNumbers_HandlesCorrectly) {
    auto result = callSum({Value(1.5), Value(2.25), Value(0.25)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(SumFunctionTest, LargeNumbers_HandlesCorrectly) {
    auto result = callSum({Value(1000000.0), Value(2000000.0), Value(3000000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6000000.0, result.asNumber());
}