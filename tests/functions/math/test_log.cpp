#include <gtest/gtest.h>
#include <velox/formulas/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class LogFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callLog(const std::vector<Value>& args) {
        return builtin::log_function(args, context);
    }
};

TEST_F(LogFunctionTest, NoArguments_ReturnsError) {
    auto result = callLog({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LogFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callLog({Value(10.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LogFunctionTest, OneArgument_BaseTenLogarithm) {
    auto result = callLog({Value(100.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2.0, result.asNumber(), 1e-10);
}

TEST_F(LogFunctionTest, OneArgument_BaseTenLogarithmOfOne) {
    auto result = callLog({Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(LogFunctionTest, OneArgument_BaseTenLogarithmOfTenth) {
    auto result = callLog({Value(0.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-1.0, result.asNumber(), 1e-10);
}

TEST_F(LogFunctionTest, TwoArguments_BaseTwoLogarithm) {
    auto result = callLog({Value(8.0), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(3.0, result.asNumber(), 1e-10);
}

TEST_F(LogFunctionTest, TwoArguments_BaseThreeLogarithm) {
    auto result = callLog({Value(9.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2.0, result.asNumber(), 1e-10);
}

TEST_F(LogFunctionTest, TwoArguments_NaturalLogarithm) {
    auto result = callLog({Value(2.718281828459045), Value(2.718281828459045)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(LogFunctionTest, NegativeNumber_ReturnsError) {
    auto result = callLog({Value(-1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(LogFunctionTest, Zero_ReturnsError) {
    auto result = callLog({Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(LogFunctionTest, NegativeBase_ReturnsError) {
    auto result = callLog({Value(10.0), Value(-2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(LogFunctionTest, ZeroBase_ReturnsError) {
    auto result = callLog({Value(10.0), Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(LogFunctionTest, BaseOne_ReturnsError) {
    auto result = callLog({Value(10.0), Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(LogFunctionTest, FirstArgumentNonNumeric_ReturnsError) {
    auto result = callLog({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LogFunctionTest, SecondArgumentNonNumeric_ReturnsError) {
    auto result = callLog({Value(10.0), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LogFunctionTest, FirstArgumentNumericText_Works) {
    auto result = callLog({Value("100.0")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2.0, result.asNumber(), 1e-10);
}

TEST_F(LogFunctionTest, SecondArgumentNumericText_Works) {
    auto result = callLog({Value(8.0), Value("2.0")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(3.0, result.asNumber(), 1e-10);
}

TEST_F(LogFunctionTest, ErrorInput_PropagatesError) {
    auto result = callLog({Value::error(ErrorType::NUM_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(LogFunctionTest, SecondArgumentError_PropagatesError) {
    auto result = callLog({Value(10.0), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(LogFunctionTest, LargeNumber_ReturnsCorrectValue) {
    auto result = callLog({Value(1000000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(6.0, result.asNumber(), 1e-10);
}

TEST_F(LogFunctionTest, SmallNumber_ReturnsCorrectValue) {
    auto result = callLog({Value(0.001)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-3.0, result.asNumber(), 1e-10);
}