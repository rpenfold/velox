#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class ExpFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callExp(const std::vector<Value>& args) {
        return builtin::exp_function(args, context);
    }
};

TEST_F(ExpFunctionTest, NoArguments_ReturnsError) {
    auto result = callExp({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ExpFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callExp({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ExpFunctionTest, Zero_ReturnsOne) {
    auto result = callExp({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(ExpFunctionTest, One_ReturnsE) {
    auto result = callExp({Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_E, result.asNumber(), 1e-10);
}

TEST_F(ExpFunctionTest, NegativeOne_ReturnsOneOverE) {
    auto result = callExp({Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0 / M_E, result.asNumber(), 1e-10);
}

TEST_F(ExpFunctionTest, LnTwo_ReturnsTwo) {
    auto result = callExp({Value(std::log(2.0))});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2.0, result.asNumber(), 1e-10);
}

TEST_F(ExpFunctionTest, LargePositive_ReturnsLargeValue) {
    auto result = callExp({Value(10.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::exp(10.0), result.asNumber(), 1e-10);
}

TEST_F(ExpFunctionTest, LargeNegative_ReturnsSmallValue) {
    auto result = callExp({Value(-10.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::exp(-10.0), result.asNumber(), 1e-10);
}

TEST_F(ExpFunctionTest, TrueBoolean_ReturnsExpOfOne) {
    auto result = callExp({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_E, result.asNumber(), 1e-10);
}

TEST_F(ExpFunctionTest, FalseBoolean_ReturnsExpOfZero) {
    auto result = callExp({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(ExpFunctionTest, NonNumericText_ReturnsError) {
    auto result = callExp({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ExpFunctionTest, NumericText_ReturnsExp) {
    auto result = callExp({Value("1.0")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_E, result.asNumber(), 1e-10);
}

TEST_F(ExpFunctionTest, ErrorInput_PropagatesError) {
    auto result = callExp({Value::error(ErrorType::NUM_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}