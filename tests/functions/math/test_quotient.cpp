#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class QuotientFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callQuotient(const std::vector<Value>& args) {
        return builtin::quotient(args, context);
    }
};

TEST_F(QuotientFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callQuotient({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());

    result = callQuotient({Value(10.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(QuotientFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callQuotient({Value(10.0), Value(3.0), Value(1.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(QuotientFunctionTest, BasicDivision_ReturnsIntegerPart) {
    auto result = callQuotient({Value(10.0), Value(3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // 10/3 = 3.333..., truncated to 3
}

TEST_F(QuotientFunctionTest, ExactDivision_ReturnsExactResult) {
    auto result = callQuotient({Value(15.0), Value(3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());  // 15/3 = 5
}

TEST_F(QuotientFunctionTest, DivisionByZero_ReturnsError) {
    auto result = callQuotient({Value(10.0), Value(0.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(QuotientFunctionTest, NegativeNumerator_TruncatesTowardZero) {
    auto result = callQuotient({Value(-10.0), Value(3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());  // -10/3 = -3.333..., truncated to -3
}

TEST_F(QuotientFunctionTest, NegativeDenominator_TruncatesTowardZero) {
    auto result = callQuotient({Value(10.0), Value(-3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());  // 10/-3 = -3.333..., truncated to -3
}

TEST_F(QuotientFunctionTest, BothNegative_ReturnsPositive) {
    auto result = callQuotient({Value(-10.0), Value(-3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // -10/-3 = 3.333..., truncated to 3
}

TEST_F(QuotientFunctionTest, DecimalInputs_WorksCorrectly) {
    auto result = callQuotient({Value(10.5), Value(2.5)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());  // 10.5/2.5 = 4.2, truncated to 4
}

TEST_F(QuotientFunctionTest, ZeroNumerator_ReturnsZero) {
    auto result = callQuotient({Value(0.0), Value(5.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(QuotientFunctionTest, FractionalResult_TruncatedCorrectly) {
    auto result = callQuotient({Value(7.0), Value(2.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // 7/2 = 3.5, truncated to 3
}

TEST_F(QuotientFunctionTest, NegativeFractionalResult_TruncatedTowardZero) {
    auto result = callQuotient({Value(-7.0), Value(2.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());  // -7/2 = -3.5, truncated to -3 (toward zero)
}

TEST_F(QuotientFunctionTest, ErrorInput_PropagatesError) {
    auto result = callQuotient({Value::error(ErrorType::VALUE_ERROR), Value(3.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());

    result = callQuotient({Value(10.0), Value::error(ErrorType::NUM_ERROR)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(QuotientFunctionTest, TextInput_ConvertsIfNumeric) {
    auto result = callQuotient({Value("10"), Value("3")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(QuotientFunctionTest, BooleanInput_ConvertsCorrectly) {
    auto result = callQuotient({Value(true), Value(1.0)});  // TRUE = 1
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}
