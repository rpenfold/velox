#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class RadiansFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callRadians(const std::vector<Value>& args) {
        return builtin::radians_function(args, context);
    }
};

TEST_F(RadiansFunctionTest, NoArguments_ReturnsError) {
    auto result = callRadians({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RadiansFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callRadians({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RadiansFunctionTest, ZeroDegrees_ReturnsZeroRadians) {
    auto result = callRadians({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(RadiansFunctionTest, OneEightyDegrees_ReturnsPiRadians) {
    auto result = callRadians({Value(180.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI, result.asNumber(), 1e-10);
}

TEST_F(RadiansFunctionTest, NinetyDegrees_ReturnsPiOverTwoRadians) {
    auto result = callRadians({Value(90.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI / 2.0, result.asNumber(), 1e-10);
}

TEST_F(RadiansFunctionTest, FortyFiveDegrees_ReturnsPiOverFourRadians) {
    auto result = callRadians({Value(45.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI / 4.0, result.asNumber(), 1e-10);
}

TEST_F(RadiansFunctionTest, ThreeSixtyDegrees_ReturnsTwoPiRadians) {
    auto result = callRadians({Value(360.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2.0 * M_PI, result.asNumber(), 1e-10);
}

TEST_F(RadiansFunctionTest, NegativeDegrees_ReturnsNegativeRadians) {
    auto result = callRadians({Value(-90.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-M_PI / 2.0, result.asNumber(), 1e-10);
}

TEST_F(RadiansFunctionTest, LargeDegrees_ReturnsCorrectRadians) {
    auto result = callRadians({Value(1800.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(10.0 * M_PI, result.asNumber(), 1e-10);
}

TEST_F(RadiansFunctionTest, TrueBoolean_ReturnsRadiansOfOne) {
    auto result = callRadians({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI / 180.0, result.asNumber(), 1e-10);
}

TEST_F(RadiansFunctionTest, FalseBoolean_ReturnsZeroRadians) {
    auto result = callRadians({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(RadiansFunctionTest, NonNumericText_ReturnsError) {
    auto result = callRadians({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RadiansFunctionTest, NumericText_ReturnsRadians) {
    auto result = callRadians({Value("180.0")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI, result.asNumber(), 1e-10);
}

TEST_F(RadiansFunctionTest, ErrorInput_PropagatesError) {
    auto result = callRadians({Value::error(ErrorType::NUM_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}