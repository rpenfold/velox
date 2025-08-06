#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class TanFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callTan(const std::vector<Value>& args) {
        return builtin::tan_function(args, context);
    }
};

TEST_F(TanFunctionTest, NoArguments_ReturnsError) {
    auto result = callTan({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TanFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callTan({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TanFunctionTest, Zero_ReturnsZero) {
    auto result = callTan({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(TanFunctionTest, PiOverFour_ReturnsOne) {
    auto result = callTan({Value(M_PI / 4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(TanFunctionTest, PiOverThree_ReturnsSqrtThree) {
    auto result = callTan({Value(M_PI / 3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::sqrt(3.0), result.asNumber(), 1e-10);
}

TEST_F(TanFunctionTest, PiOverSix_ReturnsOneOverSqrtThree) {
    auto result = callTan({Value(M_PI / 6.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0 / std::sqrt(3.0), result.asNumber(), 1e-10);
}

TEST_F(TanFunctionTest, NegativeAngle_ReturnsNegativeTangent) {
    auto result = callTan({Value(-M_PI / 4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-1.0, result.asNumber(), 1e-10);
}

TEST_F(TanFunctionTest, PiOverTwo_ReturnsInfinity) {
    auto result = callTan({Value(M_PI / 2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_TRUE(std::isinf(result.asNumber()));
}

TEST_F(TanFunctionTest, ThreePiOverTwo_ReturnsInfinity) {
    auto result = callTan({Value(3.0 * M_PI / 2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_TRUE(std::isinf(result.asNumber()));
}

TEST_F(TanFunctionTest, TrueBoolean_ReturnsTangentOfOne) {
    auto result = callTan({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(std::tan(1.0), result.asNumber(), 1e-10);
}

TEST_F(TanFunctionTest, FalseBoolean_ReturnsTangentOfZero) {
    auto result = callTan({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(TanFunctionTest, NonNumericText_ReturnsError) {
    auto result = callTan({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TanFunctionTest, NumericText_ReturnsTangent) {
    auto result = callTan({Value("0.7853981633974483")});  // π/4

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(TanFunctionTest, ErrorInput_PropagatesError) {
    auto result = callTan({Value::error(ErrorType::NUM_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}