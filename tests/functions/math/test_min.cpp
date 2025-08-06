#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class MinFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callMin(const std::vector<Value>& args) {
        return builtin::min(args, context);
    }
};

TEST_F(MinFunctionTest, EmptyArguments_ReturnsError) {
    auto result = callMin({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MinFunctionTest, SingleNumber_ReturnsSameNumber) {
    auto result = callMin({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(MinFunctionTest, MultipleNumbers_ReturnsSmallest) {
    auto result = callMin({Value(10.0), Value(1.0), Value(5.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(MinFunctionTest, NegativeNumbers_ReturnsSmallest) {
    auto result = callMin({Value(-1.0), Value(-5.0), Value(-10.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-10.0, result.asNumber());
}

TEST_F(MinFunctionTest, MixedPositiveNegative_ReturnsSmallest) {
    auto result = callMin({Value(5.0), Value(-3.0), Value(10.0), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());
}

TEST_F(MinFunctionTest, TextValues_ReturnsSmallestLexicographically) {
    auto result = callMin({Value("cherry"), Value("apple"), Value("banana")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("apple", result.asText());
}

TEST_F(MinFunctionTest, MixedNumbersAndText_ReturnsSmallestOverall) {
    auto result = callMin({Value(5.0), Value("apple"), Value(10.0)});

    // Numbers are typically smaller than text in comparison
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(MinFunctionTest, BooleanValues_ReturnsSmallest) {
    auto result = callMin({Value(true), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(MinFunctionTest, WithEmptyValues_IgnoresEmpty) {
    auto result = callMin({Value(10.0), Value::empty(), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(MinFunctionTest, AllEmptyValues_ReturnsZero) {
    auto result = callMin({Value::empty(), Value::empty()});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(MinFunctionTest, WithErrorValue_PropagatesError) {
    auto result = callMin({Value(5.0), Value::error(ErrorType::VALUE_ERROR), Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MinFunctionTest, DecimalNumbers_ReturnsSmallest) {
    auto result = callMin({Value(3.15), Value(3.14), Value(2.71)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.71, result.asNumber());
}

TEST_F(MinFunctionTest, ZeroAndNegative_ReturnsSmallest) {
    auto result = callMin({Value(0.0), Value(-0.1), Value(0.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-0.1, result.asNumber());
}