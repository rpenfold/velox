#include <gtest/gtest.h>
#include "velox/formulas/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class SignFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callSign(const std::vector<Value>& args) {
        return sign(args, context);
    }
};

// Argument validation tests
TEST_F(SignFunctionTest, NoArguments_ReturnsError) {
    auto result = callSign({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SignFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callSign({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Basic functionality tests
TEST_F(SignFunctionTest, PositiveNumber_ReturnsOne) {
    auto result = callSign({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SignFunctionTest, NegativeNumber_ReturnsMinusOne) {
    auto result = callSign({Value(-3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());
}

TEST_F(SignFunctionTest, Zero_ReturnsZero) {
    auto result = callSign({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(SignFunctionTest, LargePositiveNumber_ReturnsOne) {
    auto result = callSign({Value(1000000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SignFunctionTest, LargeNegativeNumber_ReturnsMinusOne) {
    auto result = callSign({Value(-1000000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());
}

TEST_F(SignFunctionTest, SmallPositiveNumber_ReturnsOne) {
    auto result = callSign({Value(0.0001)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SignFunctionTest, SmallNegativeNumber_ReturnsMinusOne) {
    auto result = callSign({Value(-0.0001)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());
}

// Type conversion tests
TEST_F(SignFunctionTest, TrueBoolean_ReturnsOne) {
    auto result = callSign({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SignFunctionTest, FalseBoolean_ReturnsZero) {
    auto result = callSign({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(SignFunctionTest, NumericText_ReturnsCorrectSign) {
    auto result = callSign({Value("-5.5")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());
}

TEST_F(SignFunctionTest, NonNumericText_ReturnsError) {
    auto result = callSign({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Error handling tests
TEST_F(SignFunctionTest, ErrorInput_PropagatesError) {
    auto result = callSign({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

// Edge cases
TEST_F(SignFunctionTest, NegativeZero_ReturnsZero) {
    auto result = callSign({Value(-0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}