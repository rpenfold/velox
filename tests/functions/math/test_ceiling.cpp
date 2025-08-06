#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class CeilingFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callCeiling(const std::vector<Value>& args) {
        return ceiling_function(args, context);
    }
};

// Argument validation tests
TEST_F(CeilingFunctionTest, NoArguments_ReturnsError) {
    auto result = callCeiling({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(CeilingFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callCeiling({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Single argument tests
TEST_F(CeilingFunctionTest, PositiveDecimal_RoundsUp) {
    auto result = callCeiling({Value(3.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(CeilingFunctionTest, NegativeDecimal_RoundsUp) {
    auto result = callCeiling({Value(-3.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());
}

TEST_F(CeilingFunctionTest, PositiveInteger_ReturnsUnchanged) {
    auto result = callCeiling({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(CeilingFunctionTest, Zero_ReturnsZero) {
    auto result = callCeiling({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

// Two argument tests (significance)
TEST_F(CeilingFunctionTest, PositiveNumberPositiveSignificance_RoundsUp) {
    auto result = callCeiling({Value(2.5), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(CeilingFunctionTest, RoundToNearestFive_RoundsUp) {
    auto result = callCeiling({Value(12.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(15.0, result.asNumber());
}

TEST_F(CeilingFunctionTest, RoundToNearestTen_RoundsUp) {
    auto result = callCeiling({Value(23.0), Value(10.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(30.0, result.asNumber());
}

TEST_F(CeilingFunctionTest, ExactMultiple_ReturnsUnchanged) {
    auto result = callCeiling({Value(15.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(15.0, result.asNumber());
}

TEST_F(CeilingFunctionTest, NegativeNumberNegativeSignificance_RoundsCorrectly) {
    auto result = callCeiling({Value(-2.5), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-2.0, result.asNumber());
}

TEST_F(CeilingFunctionTest, DecimalSignificance_RoundsCorrectly) {
    auto result = callCeiling({Value(2.13), Value(0.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2.2, result.asNumber(), 1e-10);
}

// Error cases
TEST_F(CeilingFunctionTest, ZeroSignificance_ReturnsError) {
    auto result = callCeiling({Value(5.0), Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(CeilingFunctionTest, PositiveNumberNegativeSignificance_ReturnsError) {
    auto result = callCeiling({Value(5.0), Value(-1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// Type conversion tests
TEST_F(CeilingFunctionTest, BooleanInputs_ConvertsToNumbers) {
    auto result = callCeiling({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(CeilingFunctionTest, NumericTextInputs_ConvertsToNumbers) {
    auto result = callCeiling({Value("2.3"), Value("1")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(CeilingFunctionTest, NonNumericText_ReturnsError) {
    auto result = callCeiling({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Error handling tests
TEST_F(CeilingFunctionTest, ErrorInput_PropagatesError) {
    auto result = callCeiling({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}