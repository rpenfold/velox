#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class MaxFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callMax(const std::vector<Value>& args) {
        return builtin::max(args, context);
    }
};

TEST_F(MaxFunctionTest, EmptyArguments_ReturnsError) {
    auto result = callMax({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MaxFunctionTest, SingleNumber_ReturnsSameNumber) {
    auto result = callMax({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(MaxFunctionTest, MultipleNumbers_ReturnsLargest) {
    auto result = callMax({Value(1.0), Value(10.0), Value(5.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(10.0, result.asNumber());
}

TEST_F(MaxFunctionTest, NegativeNumbers_ReturnsLargest) {
    auto result = callMax({Value(-10.0), Value(-5.0), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());
}

TEST_F(MaxFunctionTest, MixedPositiveNegative_ReturnsLargest) {
    auto result = callMax({Value(-5.0), Value(3.0), Value(-10.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(MaxFunctionTest, TextValues_ReturnsLargestLexicographically) {
    auto result = callMax({Value("apple"), Value("banana"), Value("cherry")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("cherry", result.asText());
}

TEST_F(MaxFunctionTest, MixedNumbersAndText_TextWinsComparison) {
    auto result = callMax({Value(5.0), Value("hello"), Value(10.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(MaxFunctionTest, BooleanValues_ReturnsLargest) {
    auto result = callMax({Value(true), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(MaxFunctionTest, WithEmptyValues_IgnoresEmpty) {
    auto result = callMax({Value(5.0), Value::empty(), Value(10.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(10.0, result.asNumber());
}

TEST_F(MaxFunctionTest, AllEmptyValues_ReturnsZero) {
    auto result = callMax({Value::empty(), Value::empty()});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(MaxFunctionTest, WithErrorValue_PropagatesError) {
    auto result = callMax({Value(1.0), Value::error(ErrorType::VALUE_ERROR), Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MaxFunctionTest, DecimalNumbers_ReturnsLargest) {
    auto result = callMax({Value(3.14), Value(2.71), Value(3.15)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.15, result.asNumber());
}