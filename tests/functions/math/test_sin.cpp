#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class SinFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callSin(const std::vector<Value>& args) {
        return builtin::sin_function(args, context);
    }
};

TEST_F(SinFunctionTest, NoArguments_ReturnsError) {
    auto result = callSin({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SinFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callSin({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SinFunctionTest, Zero_ReturnsZero) {
    auto result = callSin({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(SinFunctionTest, PiOverTwo_ReturnsOne) {
    auto result = callSin({Value(M_PI / 2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, Pi_ReturnsZero) {
    auto result = callSin({Value(M_PI)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, ThreePiOverTwo_ReturnsNegativeOne) {
    auto result = callSin({Value(3.0 * M_PI / 2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-1.0, result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, TwoPi_ReturnsZero) {
    auto result = callSin({Value(2.0 * M_PI)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, PiOverSix_ReturnsHalf) {
    auto result = callSin({Value(M_PI / 6.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.5, result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, PiOverFour_ReturnsSqrtTwoOverTwo) {
    auto result = callSin({Value(M_PI / 4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::sqrt(2.0) / 2.0, result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, PiOverThree_ReturnsSqrtThreeOverTwo) {
    auto result = callSin({Value(M_PI / 3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::sqrt(3.0) / 2.0, result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, NegativeAngle_ReturnsNegativeSine) {
    auto result = callSin({Value(-M_PI / 2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-1.0, result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, LargeAngle_ReturnsCorrectValue) {
    auto result = callSin({Value(10.0 * M_PI)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, TrueBoolean_ReturnsSineOfOne) {
    auto result = callSin({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::sin(1.0), result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, FalseBoolean_ReturnsSineOfZero) {
    auto result = callSin({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(SinFunctionTest, NonNumericText_ReturnsError) {
    auto result = callSin({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SinFunctionTest, NumericText_ReturnsSine) {
    auto result = callSin({Value("1.5707963267948966")}); // π/2

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(SinFunctionTest, ErrorInput_PropagatesError) {
    auto result = callSin({Value::error(ErrorType::NUM_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}