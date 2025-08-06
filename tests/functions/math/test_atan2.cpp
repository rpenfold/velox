#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class Atan2FunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callAtan2(const std::vector<Value>& args) {
        return builtin::atan2_function(args, context);
    }
};

TEST_F(Atan2FunctionTest, NoArguments_ReturnsError) {
    auto result = callAtan2({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(Atan2FunctionTest, OneArgument_ReturnsError) {
    auto result = callAtan2({Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(Atan2FunctionTest, TooManyArguments_ReturnsError) {
    auto result = callAtan2({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(Atan2FunctionTest, PositiveYPositiveX_ReturnsPositiveAngle) {
    auto result = callAtan2({Value(1.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI / 4.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, PositiveYNegativeX_ReturnsObtuseAngle) {
    auto result = callAtan2({Value(1.0), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(3.0 * M_PI / 4.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, NegativeYNegativeX_ReturnsNegativeObtuseAngle) {
    auto result = callAtan2({Value(-1.0), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-3.0 * M_PI / 4.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, NegativeYPositiveX_ReturnsNegativeAcuteAngle) {
    auto result = callAtan2({Value(-1.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-M_PI / 4.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, ZeroYPositiveX_ReturnsZero) {
    auto result = callAtan2({Value(0.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, PositiveYZeroX_ReturnsPiOverTwo) {
    auto result = callAtan2({Value(1.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI / 2.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, NegativeYZeroX_ReturnsNegativePiOverTwo) {
    auto result = callAtan2({Value(-1.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-M_PI / 2.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, ZeroYZeroX_ReturnsZero) {
    auto result = callAtan2({Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, LargeValues_ReturnsCorrectAngle) {
    auto result = callAtan2({Value(1000.0), Value(1000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI / 4.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, SmallValues_ReturnsCorrectAngle) {
    auto result = callAtan2({Value(0.001), Value(0.001)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI / 4.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, FirstArgumentNonNumeric_ReturnsError) {
    auto result = callAtan2({Value("hello"), Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(Atan2FunctionTest, SecondArgumentNonNumeric_ReturnsError) {
    auto result = callAtan2({Value(1.0), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(Atan2FunctionTest, FirstArgumentNumericText_Works) {
    auto result = callAtan2({Value("1.0"), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI / 4.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, SecondArgumentNumericText_Works) {
    auto result = callAtan2({Value(1.0), Value("1.0")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(M_PI / 4.0, result.asNumber(), 1e-10);
}

TEST_F(Atan2FunctionTest, ErrorInput_PropagatesError) {
    auto result = callAtan2({Value::error(ErrorType::NUM_ERROR), Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Atan2FunctionTest, SecondArgumentError_PropagatesError) {
    auto result = callAtan2({Value(1.0), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}