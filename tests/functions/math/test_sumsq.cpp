#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class SumsqFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callSumsq(const std::vector<Value>& args) {
        return builtin::sumsq(args, context);
    }
};

TEST_F(SumsqFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callSumsq({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SumsqFunctionTest, SingleNumber_ReturnsSquare) {
    auto result = callSumsq({Value(3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(9.0, result.asNumber());
}

TEST_F(SumsqFunctionTest, MultipleNumbers_ReturnsSumOfSquares) {
    auto result = callSumsq({Value(2.0), Value(3.0), Value(4.0)});
    ASSERT_TRUE(result.isNumber());
    // 2^2 + 3^2 + 4^2 = 4 + 9 + 16 = 29
    EXPECT_DOUBLE_EQ(29.0, result.asNumber());
}

TEST_F(SumsqFunctionTest, NegativeNumbers_WorksCorrectly) {
    auto result = callSumsq({Value(-2.0), Value(-3.0)});
    ASSERT_TRUE(result.isNumber());
    // (-2)^2 + (-3)^2 = 4 + 9 = 13
    EXPECT_DOUBLE_EQ(13.0, result.asNumber());
}

TEST_F(SumsqFunctionTest, MixedPositiveNegative_WorksCorrectly) {
    auto result = callSumsq({Value(-2.0), Value(3.0)});
    ASSERT_TRUE(result.isNumber());
    // (-2)^2 + 3^2 = 4 + 9 = 13
    EXPECT_DOUBLE_EQ(13.0, result.asNumber());
}

TEST_F(SumsqFunctionTest, Zero_ReturnsZero) {
    auto result = callSumsq({Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(SumsqFunctionTest, DecimalNumbers_WorksCorrectly) {
    auto result = callSumsq({Value(1.5), Value(2.5)});
    ASSERT_TRUE(result.isNumber());
    // 1.5^2 + 2.5^2 = 2.25 + 6.25 = 8.5
    EXPECT_DOUBLE_EQ(8.5, result.asNumber());
}

TEST_F(SumsqFunctionTest, NonNumericValues_IgnoredLikeExcel) {
    auto result = callSumsq({Value(2.0), Value("text"), Value(true), Value(3.0)});
    ASSERT_TRUE(result.isNumber());
    // Only 2^2 + 3^2 = 4 + 9 = 13
    EXPECT_DOUBLE_EQ(13.0, result.asNumber());
}

TEST_F(SumsqFunctionTest, AllNonNumeric_ReturnsZero) {
    auto result = callSumsq({Value("text"), Value(true), Value("")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(SumsqFunctionTest, ErrorInput_PropagatesError) {
    auto result = callSumsq({Value(2.0), Value::error(ErrorType::DIV_ZERO), Value(3.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(SumsqFunctionTest, LargeNumbers_WorksCorrectly) {
    auto result = callSumsq({Value(100.0), Value(200.0)});
    ASSERT_TRUE(result.isNumber());
    // 100^2 + 200^2 = 10000 + 40000 = 50000
    EXPECT_DOUBLE_EQ(50000.0, result.asNumber());
}

TEST_F(SumsqFunctionTest, ManyArguments_WorksCorrectly) {
    auto result = callSumsq({Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)});
    ASSERT_TRUE(result.isNumber());
    // 1^2 + 2^2 + 3^2 + 4^2 + 5^2 = 1 + 4 + 9 + 16 + 25 = 55
    EXPECT_DOUBLE_EQ(55.0, result.asNumber());
}
