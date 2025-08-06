#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class LnFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callLn(const std::vector<Value>& args) {
        return builtin::ln_function(args, context);
    }
};

TEST_F(LnFunctionTest, NoArguments_ReturnsError) {
    auto result = callLn({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LnFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callLn({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LnFunctionTest, One_ReturnsZero) {
    auto result = callLn({Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(LnFunctionTest, E_ReturnsOne) {
    auto result = callLn({Value(M_E)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(LnFunctionTest, E_Squared_ReturnsTwo) {
    auto result = callLn({Value(M_E * M_E)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2.0, result.asNumber(), 1e-10);
}

TEST_F(LnFunctionTest, OneOverE_ReturnsNegativeOne) {
    auto result = callLn({Value(1.0 / M_E)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-1.0, result.asNumber(), 1e-10);
}

TEST_F(LnFunctionTest, Two_ReturnsLnTwo) {
    auto result = callLn({Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::log(2.0), result.asNumber(), 1e-10);
}

TEST_F(LnFunctionTest, Ten_ReturnsLnTen) {
    auto result = callLn({Value(10.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::log(10.0), result.asNumber(), 1e-10);
}

TEST_F(LnFunctionTest, Zero_ReturnsError) {
    auto result = callLn({Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(LnFunctionTest, NegativeNumber_ReturnsError) {
    auto result = callLn({Value(-1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(LnFunctionTest, TrueBoolean_ReturnsLnOfOne) {
    auto result = callLn({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(LnFunctionTest, FalseBoolean_ReturnsError) {
    auto result = callLn({Value(false)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(LnFunctionTest, NonNumericText_ReturnsError) {
    auto result = callLn({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LnFunctionTest, NumericText_ReturnsLn) {
    auto result = callLn({Value("2.718281828459045")}); // e

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(LnFunctionTest, ErrorInput_PropagatesError) {
    auto result = callLn({Value::error(ErrorType::NUM_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(LnFunctionTest, LargeNumber_ReturnsCorrectValue) {
    auto result = callLn({Value(1000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::log(1000.0), result.asNumber(), 1e-10);
}

TEST_F(LnFunctionTest, SmallNumber_ReturnsCorrectValue) {
    auto result = callLn({Value(0.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::log(0.1), result.asNumber(), 1e-10);
}