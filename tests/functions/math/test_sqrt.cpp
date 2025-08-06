#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class SqrtFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callSqrt(const std::vector<Value>& args) {
        return builtin::sqrt_function(args, context);
    }
};

TEST_F(SqrtFunctionTest, NoArguments_ReturnsError) {
    auto result = callSqrt({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SqrtFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callSqrt({Value(4.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SqrtFunctionTest, PositiveNumber_ReturnsSquareRoot) {
    auto result = callSqrt({Value(9.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(SqrtFunctionTest, Zero_ReturnsZero) {
    auto result = callSqrt({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(SqrtFunctionTest, One_ReturnsOne) {
    auto result = callSqrt({Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SqrtFunctionTest, NegativeNumber_ReturnsError) {
    auto result = callSqrt({Value(-4.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(SqrtFunctionTest, DecimalNumber_ReturnsCorrectSquareRoot) {
    auto result = callSqrt({Value(2.25)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.5, result.asNumber());
}

TEST_F(SqrtFunctionTest, LargeNumber_ReturnsCorrectSquareRoot) {
    auto result = callSqrt({Value(100.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(10.0, result.asNumber());
}

TEST_F(SqrtFunctionTest, SmallDecimal_ReturnsCorrectSquareRoot) {
    auto result = callSqrt({Value(0.25)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.5, result.asNumber());
}

TEST_F(SqrtFunctionTest, TrueBooleanInput_ReturnsOne) {
    auto result = callSqrt({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SqrtFunctionTest, FalseBooleanInput_ReturnsZero) {
    auto result = callSqrt({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(SqrtFunctionTest, NumericTextInput_ReturnsSquareRoot) {
    auto result = callSqrt({Value("16")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(SqrtFunctionTest, NonNumericTextInput_ReturnsError) {
    auto result = callSqrt({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SqrtFunctionTest, ErrorInput_PropagatesError) {
    auto result = callSqrt({Value::error(ErrorType::REF_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(SqrtFunctionTest, IrrationalNumber_ReturnsApproximation) {
    auto result = callSqrt({Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::sqrt(2.0), result.asNumber(), 1e-15);
}