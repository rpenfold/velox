#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class OddFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callOdd(const std::vector<Value>& args) {
        return builtin::odd_function(args, context);
    }
};

TEST_F(OddFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callOdd({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(OddFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callOdd({Value(1.0), Value(2.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(OddFunctionTest, Zero_ReturnsOne) {
    auto result = callOdd({Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(OddFunctionTest, PositiveOddInteger_StaysSame) {
    auto result = callOdd({Value(1.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());

    result = callOdd({Value(3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(OddFunctionTest, PositiveEvenInteger_RoundsUp) {
    auto result = callOdd({Value(2.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());

    result = callOdd({Value(4.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(OddFunctionTest, PositiveDecimal_RoundsUpToNextOdd) {
    auto result = callOdd({Value(1.2)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());

    result = callOdd({Value(2.5)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());

    result = callOdd({Value(3.7)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(OddFunctionTest, NegativeOddInteger_StaysSame) {
    auto result = callOdd({Value(-1.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());

    result = callOdd({Value(-3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());
}

TEST_F(OddFunctionTest, NegativeEvenInteger_RoundsAwayFromZero) {
    auto result = callOdd({Value(-2.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());

    result = callOdd({Value(-4.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-5.0, result.asNumber());
}

TEST_F(OddFunctionTest, NegativeDecimal_RoundsAwayFromZero) {
    auto result = callOdd({Value(-1.5)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());

    result = callOdd({Value(-2.3)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());

    result = callOdd({Value(-3.7)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-5.0, result.asNumber());
}

TEST_F(OddFunctionTest, LargeNumbers_WorksCorrectly) {
    auto result = callOdd({Value(1000.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1001.0, result.asNumber());

    result = callOdd({Value(-1000.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1001.0, result.asNumber());
}

TEST_F(OddFunctionTest, ErrorInput_PropagatesError) {
    auto result = callOdd({Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(OddFunctionTest, TextInput_ConvertsIfNumeric) {
    auto result = callOdd({Value("2")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(OddFunctionTest, BooleanInput_ConvertsCorrectly) {
    auto result = callOdd({Value(true)});  // TRUE = 1
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());

    result = callOdd({Value(false)});  // FALSE = 0
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}
