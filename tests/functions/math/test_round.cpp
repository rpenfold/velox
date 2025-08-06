#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class RoundFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callRound(const std::vector<Value>& args) {
        return builtin::round_function(args, context);
    }
};

TEST_F(RoundFunctionTest, NoArguments_ReturnsError) {
    auto result = callRound({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RoundFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callRound({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RoundFunctionTest, OneArgument_RoundsToNearestInteger) {
    auto result = callRound({Value(3.14159)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(RoundFunctionTest, OneArgument_RoundsUpFromHalf) {
    auto result = callRound({Value(3.6)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(RoundFunctionTest, NegativeNumber_RoundsCorrectly) {
    auto result = callRound({Value(-3.14159)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());
}

TEST_F(RoundFunctionTest, TwoDecimalPlaces_RoundsCorrectly) {
    auto result = callRound({Value(3.14159), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.14, result.asNumber());
}

TEST_F(RoundFunctionTest, FourDecimalPlaces_RoundsCorrectly) {
    auto result = callRound({Value(3.14159), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.1416, result.asNumber());
}

TEST_F(RoundFunctionTest, NegativeDecimalPlaces_RoundsToTens) {
    auto result = callRound({Value(1234.5), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1230.0, result.asNumber());
}

TEST_F(RoundFunctionTest, NegativeDecimalPlaces_RoundsToHundreds) {
    auto result = callRound({Value(1234.5), Value(-2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1200.0, result.asNumber());
}

TEST_F(RoundFunctionTest, Zero_RoundsToZero) {
    auto result = callRound({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(RoundFunctionTest, BooleanAsPlaces_ConvertsTrueToOne) {
    auto result = callRound({Value(3.14159), Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.1, result.asNumber());
}

TEST_F(RoundFunctionTest, BooleanAsPlaces_ConvertsFalseToZero) {
    auto result = callRound({Value(3.14159), Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(RoundFunctionTest, NonNumericInput_ReturnsError) {
    auto result = callRound({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RoundFunctionTest, NonNumericPlaces_ReturnsError) {
    auto result = callRound({Value(3.14), Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RoundFunctionTest, ErrorInput_PropagatesError) {
    auto result = callRound({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(RoundFunctionTest, ErrorInPlaces_PropagatesError) {
    auto result = callRound({Value(3.14), Value::error(ErrorType::REF_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}