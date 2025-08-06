#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class IntFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callInt(const std::vector<Value>& args) {
        return int_function(args, context);
    }
};

// Argument validation tests
TEST_F(IntFunctionTest, NoArguments_ReturnsError) {
    auto result = callInt({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IntFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callInt({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Basic functionality tests
TEST_F(IntFunctionTest, PositiveDecimal_RoundsDown) {
    auto result = callInt({Value(3.7)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(IntFunctionTest, NegativeDecimal_RoundsDown) {
    auto result = callInt({Value(-3.7)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-4.0, result.asNumber());
}

TEST_F(IntFunctionTest, PositiveInteger_ReturnsUnchanged) {
    auto result = callInt({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(IntFunctionTest, NegativeInteger_ReturnsUnchanged) {
    auto result = callInt({Value(-5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-5.0, result.asNumber());
}

TEST_F(IntFunctionTest, Zero_ReturnsZero) {
    auto result = callInt({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(IntFunctionTest, SmallPositiveDecimal_RoundsDown) {
    auto result = callInt({Value(0.9)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(IntFunctionTest, SmallNegativeDecimal_RoundsDown) {
    auto result = callInt({Value(-0.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());
}

TEST_F(IntFunctionTest, LargePositiveDecimal_RoundsDown) {
    auto result = callInt({Value(999.999)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(999.0, result.asNumber());
}

TEST_F(IntFunctionTest, LargeNegativeDecimal_RoundsDown) {
    auto result = callInt({Value(-999.001)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1000.0, result.asNumber());
}

// Type conversion tests
TEST_F(IntFunctionTest, TrueBoolean_ReturnsOne) {
    auto result = callInt({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(IntFunctionTest, FalseBoolean_ReturnsZero) {
    auto result = callInt({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(IntFunctionTest, NumericText_HandlesCorrectly) {
    auto result = callInt({Value("5.7")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(IntFunctionTest, NegativeNumericText_HandlesCorrectly) {
    auto result = callInt({Value("-5.7")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-6.0, result.asNumber());
}

TEST_F(IntFunctionTest, NonNumericText_ReturnsError) {
    auto result = callInt({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Error handling tests
TEST_F(IntFunctionTest, ErrorInput_PropagatesError) {
    auto result = callInt({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

// Edge cases
TEST_F(IntFunctionTest, JustBelowInteger_RoundsDown) {
    auto result = callInt({Value(4.99999)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(IntFunctionTest, JustAboveNegativeInteger_RoundsDown) {
    auto result = callInt({Value(-4.00001)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-5.0, result.asNumber());
}