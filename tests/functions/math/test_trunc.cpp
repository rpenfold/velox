#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class TruncFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callTrunc(const std::vector<Value>& args) {
        return trunc_function(args, context);
    }
};

// Argument validation tests
TEST_F(TruncFunctionTest, NoArguments_ReturnsError) {
    auto result = callTrunc({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TruncFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callTrunc({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Single argument tests
TEST_F(TruncFunctionTest, PositiveDecimal_TruncatesToInteger) {
    auto result = callTrunc({Value(3.7)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(TruncFunctionTest, NegativeDecimal_TruncatesToInteger) {
    auto result = callTrunc({Value(-3.7)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());
}

TEST_F(TruncFunctionTest, PositiveInteger_ReturnsUnchanged) {
    auto result = callTrunc({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(TruncFunctionTest, Zero_ReturnsZero) {
    auto result = callTrunc({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

// Two argument tests (decimal places)
TEST_F(TruncFunctionTest, TwoDecimalPlaces_TruncatesCorrectly) {
    auto result = callTrunc({Value(3.14159), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.14, result.asNumber());
}

TEST_F(TruncFunctionTest, OneDecimalPlace_TruncatesCorrectly) {
    auto result = callTrunc({Value(3.789), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.7, result.asNumber());
}

TEST_F(TruncFunctionTest, ZeroDecimalPlaces_TruncatesToInteger) {
    auto result = callTrunc({Value(3.789), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(TruncFunctionTest, NegativeDecimalPlaces_TruncatesToTens) {
    auto result = callTrunc({Value(1234.567), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1230.0, result.asNumber());
}

TEST_F(TruncFunctionTest, NegativeDecimalPlaces_TruncatesToHundreds) {
    auto result = callTrunc({Value(1234.567), Value(-2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1200.0, result.asNumber());
}

// Type conversion tests
TEST_F(TruncFunctionTest, BooleanInputs_ConvertsToNumbers) {
    auto result = callTrunc({Value(true), Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(TruncFunctionTest, NumericTextInputs_ConvertsToNumbers) {
    auto result = callTrunc({Value("3.789"), Value("1")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.7, result.asNumber());
}

TEST_F(TruncFunctionTest, NonNumericTextNumber_ReturnsError) {
    auto result = callTrunc({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TruncFunctionTest, NonNumericTextDigits_ReturnsError) {
    auto result = callTrunc({Value(3.789), Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Error handling tests
TEST_F(TruncFunctionTest, ErrorInNumber_PropagatesError) {
    auto result = callTrunc({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(TruncFunctionTest, ErrorInDigits_PropagatesError) {
    auto result = callTrunc({Value(3.789), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}