#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class DegreesFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callDegrees(const std::vector<Value>& args) {
        return builtin::degrees_function(args, context);
    }
};

TEST_F(DegreesFunctionTest, NoArguments_ReturnsError) {
    auto result = callDegrees({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(DegreesFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callDegrees({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(DegreesFunctionTest, ZeroRadians_ReturnsZeroDegrees) {
    auto result = callDegrees({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(DegreesFunctionTest, PiRadians_Returns180Degrees) {
    auto result = callDegrees({Value(M_PI)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(180.0, result.asNumber(), 1e-10);
}

TEST_F(DegreesFunctionTest, PiOverTwoRadians_Returns90Degrees) {
    auto result = callDegrees({Value(M_PI / 2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(90.0, result.asNumber(), 1e-10);
}

TEST_F(DegreesFunctionTest, PiOverFourRadians_Returns45Degrees) {
    auto result = callDegrees({Value(M_PI / 4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(45.0, result.asNumber(), 1e-10);
}

TEST_F(DegreesFunctionTest, TwoPiRadians_Returns360Degrees) {
    auto result = callDegrees({Value(2.0 * M_PI)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(360.0, result.asNumber(), 1e-10);
}

TEST_F(DegreesFunctionTest, NegativeRadians_ReturnsNegativeDegrees) {
    auto result = callDegrees({Value(-M_PI / 2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-90.0, result.asNumber(), 1e-10);
}

TEST_F(DegreesFunctionTest, LargeRadians_ReturnsCorrectDegrees) {
    auto result = callDegrees({Value(10.0 * M_PI)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1800.0, result.asNumber(), 1e-10);
}

TEST_F(DegreesFunctionTest, TrueBoolean_ReturnsDegreesOfOne) {
    auto result = callDegrees({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(180.0 / M_PI, result.asNumber(), 1e-10);
}

TEST_F(DegreesFunctionTest, FalseBoolean_ReturnsZeroDegrees) {
    auto result = callDegrees({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(DegreesFunctionTest, NonNumericText_ReturnsError) {
    auto result = callDegrees({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(DegreesFunctionTest, NumericText_ReturnsDegrees) {
    auto result = callDegrees({Value("3.141592653589793")});  // π

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(180.0, result.asNumber(), 1e-10);
}

TEST_F(DegreesFunctionTest, ErrorInput_PropagatesError) {
    auto result = callDegrees({Value::error(ErrorType::NUM_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}