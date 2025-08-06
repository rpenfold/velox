#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class TimeFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callTime(const std::vector<Value>& args) {
        return time_function(args, context);
    }
};

// Basic functionality tests
TEST_F(TimeFunctionTest, ValidTime_ReturnsNumericFraction) {
    auto result = callTime({Value(12.0), Value(30.0), Value(45.0)});

    EXPECT_TRUE(result.isNumber());
    
    // 12:30:45 = 12*3600 + 30*60 + 45 = 45045 seconds
    // 45045 / 86400 = 0.5214... (fraction of day)
    double expected = 45045.0 / 86400.0;
    EXPECT_NEAR(expected, result.asNumber(), 1e-6);
}

TEST_F(TimeFunctionTest, Midnight_ReturnsZero) {
    auto result = callTime({Value(0.0), Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(TimeFunctionTest, Noon_ReturnsHalf) {
    auto result = callTime({Value(12.0), Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.5, result.asNumber());
}

TEST_F(TimeFunctionTest, AlmostMidnight_ReturnsNearOne) {
    auto result = callTime({Value(23.0), Value(59.0), Value(59.0)});

    EXPECT_TRUE(result.isNumber());
    
    // 23:59:59 = 23*3600 + 59*60 + 59 = 86399 seconds
    // 86399 / 86400 = 0.999988... (very close to 1)
    double expected = 86399.0 / 86400.0;
    EXPECT_NEAR(expected, result.asNumber(), 1e-6);
}

// Argument validation tests
TEST_F(TimeFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callTime({Value(12.0), Value(30.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TimeFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callTime({Value(12.0), Value(30.0), Value(45.0), Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TimeFunctionTest, NoArguments_ReturnsError) {
    auto result = callTime({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Invalid input tests
TEST_F(TimeFunctionTest, NonNumericHour_ReturnsError) {
    auto result = callTime({Value("text"), Value(30.0), Value(45.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TimeFunctionTest, NonNumericMinute_ReturnsError) {
    auto result = callTime({Value(12.0), Value("text"), Value(45.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TimeFunctionTest, NonNumericSecond_ReturnsError) {
    auto result = callTime({Value(12.0), Value(30.0), Value("text")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Range validation tests
TEST_F(TimeFunctionTest, HourTooLow_ReturnsError) {
    auto result = callTime({Value(-1.0), Value(30.0), Value(45.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(TimeFunctionTest, HourTooHigh_ReturnsError) {
    auto result = callTime({Value(24.0), Value(30.0), Value(45.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(TimeFunctionTest, MinuteTooLow_ReturnsError) {
    auto result = callTime({Value(12.0), Value(-1.0), Value(45.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(TimeFunctionTest, MinuteTooHigh_ReturnsError) {
    auto result = callTime({Value(12.0), Value(60.0), Value(45.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(TimeFunctionTest, SecondTooLow_ReturnsError) {
    auto result = callTime({Value(12.0), Value(30.0), Value(-1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(TimeFunctionTest, SecondTooHigh_ReturnsError) {
    auto result = callTime({Value(12.0), Value(30.0), Value(60.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// Boundary tests
TEST_F(TimeFunctionTest, MaxValidTime_ReturnsValidFraction) {
    auto result = callTime({Value(23.0), Value(59.0), Value(59.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_LT(result.asNumber(), 1.0);
}

TEST_F(TimeFunctionTest, MinValidTime_ReturnsZero) {
    auto result = callTime({Value(0.0), Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

// Error propagation tests
TEST_F(TimeFunctionTest, ErrorInHour_PropagatesError) {
    auto result = callTime({Value::error(ErrorType::DIV_ZERO), Value(30.0), Value(45.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(TimeFunctionTest, ErrorInMinute_PropagatesError) {
    auto result = callTime({Value(12.0), Value::error(ErrorType::DIV_ZERO), Value(45.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(TimeFunctionTest, ErrorInSecond_PropagatesError) {
    auto result = callTime({Value(12.0), Value(30.0), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}