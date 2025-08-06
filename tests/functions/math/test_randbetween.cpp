#include <gtest/gtest.h>
#include <cmath>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class RandBetweenFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callRandBetween(const std::vector<Value>& args) {
        return randbetween(args, context);
    }
};

// Argument validation tests
TEST_F(RandBetweenFunctionTest, NoArguments_ReturnsError) {
    auto result = callRandBetween({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RandBetweenFunctionTest, OneArgument_ReturnsError) {
    auto result = callRandBetween({Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RandBetweenFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callRandBetween({Value(1.0), Value(10.0), Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RandBetweenFunctionTest, BottomGreaterThanTop_ReturnsError) {
    auto result = callRandBetween({Value(10.0), Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// Basic functionality tests
TEST_F(RandBetweenFunctionTest, ValidRange_ReturnsNumberInRange) {
    auto result = callRandBetween({Value(1.0), Value(10.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_GE(result.asNumber(), 1.0);
    EXPECT_LE(result.asNumber(), 10.0);
}

TEST_F(RandBetweenFunctionTest, SameBottomAndTop_ReturnsThatNumber) {
    auto result = callRandBetween({Value(5.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(RandBetweenFunctionTest, NegativeRange_ReturnsNumberInRange) {
    auto result = callRandBetween({Value(-10.0), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_GE(result.asNumber(), -10.0);
    EXPECT_LE(result.asNumber(), -1.0);
}

TEST_F(RandBetweenFunctionTest, CrossingZero_ReturnsNumberInRange) {
    auto result = callRandBetween({Value(-5.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_GE(result.asNumber(), -5.0);
    EXPECT_LE(result.asNumber(), 5.0);
}

TEST_F(RandBetweenFunctionTest, DecimalInputs_TruncatesToIntegers) {
    auto result = callRandBetween({Value(1.7), Value(3.9)});

    EXPECT_TRUE(result.isNumber());
    // Should be between floor(1.7)=1 and floor(3.9)=3
    EXPECT_GE(result.asNumber(), 1.0);
    EXPECT_LE(result.asNumber(), 3.0);

    // Should be an integer
    EXPECT_EQ(result.asNumber(), std::floor(result.asNumber()));
}

// Randomness tests
TEST_F(RandBetweenFunctionTest, MultipleCalls_ReturnDifferentValues) {
    std::set<double> values;

    // Generate many random numbers in a reasonable range
    for (int i = 0; i < 50; ++i) {
        auto result = callRandBetween({Value(1.0), Value(100.0)});
        EXPECT_TRUE(result.isNumber());
        values.insert(result.asNumber());
    }

    // Should have generated multiple different values
    EXPECT_GT(values.size(), 10u);
}

TEST_F(RandBetweenFunctionTest, SmallRange_CoversAllValues) {
    std::set<double> values;

    // Generate many random numbers in small range [1, 3]
    for (int i = 0; i < 200; ++i) {
        auto result = callRandBetween({Value(1.0), Value(3.0)});
        EXPECT_TRUE(result.isNumber());
        values.insert(result.asNumber());
    }

    // Should see all values 1, 2, 3
    EXPECT_TRUE(values.count(1.0) > 0);
    EXPECT_TRUE(values.count(2.0) > 0);
    EXPECT_TRUE(values.count(3.0) > 0);
    EXPECT_EQ(3u, values.size());  // Only these three values
}

// Type conversion tests
TEST_F(RandBetweenFunctionTest, BooleanInputs_ConvertsToNumbers) {
    auto result = callRandBetween({Value(false), Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_GE(result.asNumber(), 0.0);
    EXPECT_LE(result.asNumber(), 1.0);
}

TEST_F(RandBetweenFunctionTest, NumericTextInputs_ConvertsToNumbers) {
    auto result = callRandBetween({Value("1"), Value("10")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_GE(result.asNumber(), 1.0);
    EXPECT_LE(result.asNumber(), 10.0);
}

TEST_F(RandBetweenFunctionTest, NonNumericTextBottom_ReturnsError) {
    auto result = callRandBetween({Value("hello"), Value(10.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RandBetweenFunctionTest, NonNumericTextTop_ReturnsError) {
    auto result = callRandBetween({Value(1.0), Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Error handling tests
TEST_F(RandBetweenFunctionTest, ErrorInBottom_PropagatesError) {
    auto result = callRandBetween({Value::error(ErrorType::DIV_ZERO), Value(10.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(RandBetweenFunctionTest, ErrorInTop_PropagatesError) {
    auto result = callRandBetween({Value(1.0), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}