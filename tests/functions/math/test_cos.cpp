#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class CosFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callCos(const std::vector<Value>& args) {
        return builtin::cos_function(args, context);
    }
};

TEST_F(CosFunctionTest, NoArguments_ReturnsError) {
    auto result = callCos({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(CosFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callCos({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(CosFunctionTest, Zero_ReturnsOne) {
    auto result = callCos({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(CosFunctionTest, PiOverTwo_ReturnsZero) {
    auto result = callCos({Value(M_PI / 2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(CosFunctionTest, Pi_ReturnsNegativeOne) {
    auto result = callCos({Value(M_PI)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-1.0, result.asNumber(), 1e-10);
}

TEST_F(CosFunctionTest, ThreePiOverTwo_ReturnsZero) {
    auto result = callCos({Value(3.0 * M_PI / 2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(CosFunctionTest, TwoPi_ReturnsOne) {
    auto result = callCos({Value(2.0 * M_PI)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(CosFunctionTest, PiOverThree_ReturnsHalf) {
    auto result = callCos({Value(M_PI / 3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.5, result.asNumber(), 1e-10);
}

TEST_F(CosFunctionTest, PiOverFour_ReturnsSqrtTwoOverTwo) {
    auto result = callCos({Value(M_PI / 4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::sqrt(2.0) / 2.0, result.asNumber(), 1e-10);
}

TEST_F(CosFunctionTest, PiOverSix_ReturnsSqrtThreeOverTwo) {
    auto result = callCos({Value(M_PI / 6.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::sqrt(3.0) / 2.0, result.asNumber(), 1e-10);
}

TEST_F(CosFunctionTest, NegativeAngle_ReturnsSameCosine) {
    auto result = callCos({Value(-M_PI / 3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.5, result.asNumber(), 1e-10);
}

TEST_F(CosFunctionTest, LargeAngle_ReturnsCorrectValue) {
    auto result = callCos({Value(10.0 * M_PI)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(CosFunctionTest, TrueBoolean_ReturnsCosineOfOne) {
    auto result = callCos({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::cos(1.0), result.asNumber(), 1e-10);
}

TEST_F(CosFunctionTest, FalseBoolean_ReturnsCosineOfZero) {
    auto result = callCos({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(CosFunctionTest, NonNumericText_ReturnsError) {
    auto result = callCos({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(CosFunctionTest, NumericText_ReturnsCosine) {
    auto result = callCos({Value("0.0")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(CosFunctionTest, ErrorInput_PropagatesError) {
    auto result = callCos({Value::error(ErrorType::NUM_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}