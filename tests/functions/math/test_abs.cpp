#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class AbsFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callAbs(const std::vector<Value>& args) {
        return builtin::abs_function(args, context);
    }
};

TEST_F(AbsFunctionTest, NoArguments_ReturnsError) {
    auto result = callAbs({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(AbsFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callAbs({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(AbsFunctionTest, PositiveNumber_ReturnsSameNumber) {
    auto result = callAbs({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(AbsFunctionTest, NegativeNumber_ReturnsPositive) {
    auto result = callAbs({Value(-5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(AbsFunctionTest, Zero_ReturnsZero) {
    auto result = callAbs({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(AbsFunctionTest, TrueBoolean_ReturnsOne) {
    auto result = callAbs({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(AbsFunctionTest, FalseBoolean_ReturnsZero) {
    auto result = callAbs({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(AbsFunctionTest, NonNumericText_ReturnsError) {
    auto result = callAbs({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(AbsFunctionTest, NumericText_ReturnsAbsoluteValue) {
    auto result = callAbs({Value("-3.5")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.5, result.asNumber());
}

TEST_F(AbsFunctionTest, ErrorInput_PropagatesError) {
    auto result = callAbs({Value::error(ErrorType::NUM_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(AbsFunctionTest, LargeNegativeNumber_ReturnsAbsoluteValue) {
    auto result = callAbs({Value(-1000000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1000000.0, result.asNumber());
}

TEST_F(AbsFunctionTest, SmallDecimal_ReturnsAbsoluteValue) {
    auto result = callAbs({Value(-0.001)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.001, result.asNumber());
}