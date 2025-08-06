#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class FloorFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callFloor(const std::vector<Value>& args) {
        return floor_function(args, context);
    }
};

// Argument validation tests
TEST_F(FloorFunctionTest, NoArguments_ReturnsError) {
    auto result = callFloor({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(FloorFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callFloor({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Single argument tests
TEST_F(FloorFunctionTest, PositiveDecimal_RoundsDown) {
    auto result = callFloor({Value(3.9)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(FloorFunctionTest, NegativeDecimal_RoundsDown) {
    auto result = callFloor({Value(-3.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-4.0, result.asNumber());
}

TEST_F(FloorFunctionTest, PositiveInteger_ReturnsUnchanged) {
    auto result = callFloor({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(FloorFunctionTest, Zero_ReturnsZero) {
    auto result = callFloor({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

// Two argument tests (significance)
TEST_F(FloorFunctionTest, PositiveNumberPositiveSignificance_RoundsDown) {
    auto result = callFloor({Value(2.9), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(FloorFunctionTest, RoundToNearestFive_RoundsDown) {
    auto result = callFloor({Value(18.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(15.0, result.asNumber());
}

TEST_F(FloorFunctionTest, RoundToNearestTen_RoundsDown) {
    auto result = callFloor({Value(27.0), Value(10.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(20.0, result.asNumber());
}

TEST_F(FloorFunctionTest, ExactMultiple_ReturnsUnchanged) {
    auto result = callFloor({Value(15.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(15.0, result.asNumber());
}

TEST_F(FloorFunctionTest, NegativeNumberNegativeSignificance_RoundsCorrectly) {
    auto result = callFloor({Value(-2.1), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());
}

TEST_F(FloorFunctionTest, DecimalSignificance_RoundsCorrectly) {
    auto result = callFloor({Value(2.17), Value(0.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2.1, result.asNumber(), 1e-10);
}

// Error cases
TEST_F(FloorFunctionTest, ZeroSignificance_ReturnsError) {
    auto result = callFloor({Value(5.0), Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(FloorFunctionTest, PositiveNumberNegativeSignificance_ReturnsError) {
    auto result = callFloor({Value(5.0), Value(-1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// Type conversion tests
TEST_F(FloorFunctionTest, BooleanInputs_ConvertsToNumbers) {
    auto result = callFloor({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(FloorFunctionTest, NumericTextInputs_ConvertsToNumbers) {
    auto result = callFloor({Value("2.7"), Value("1")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(FloorFunctionTest, NonNumericText_ReturnsError) {
    auto result = callFloor({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Error handling tests
TEST_F(FloorFunctionTest, ErrorInput_PropagatesError) {
    auto result = callFloor({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}