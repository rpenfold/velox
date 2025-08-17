#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class MedianFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callMedian(const std::vector<Value>& args) {
        return builtin::median(args, context);
    }
};

TEST_F(MedianFunctionTest, EmptyArguments_ReturnsError) {
    auto result = callMedian({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MedianFunctionTest, SingleNumber_ReturnsSameNumber) {
    auto result = callMedian({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(MedianFunctionTest, TwoNumbers_ReturnsAverage) {
    auto result = callMedian({Value(1.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(MedianFunctionTest, ThreeNumbers_ReturnsMiddleValue) {
    auto result = callMedian({Value(1.0), Value(3.0), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(MedianFunctionTest, FourNumbers_ReturnsAverageOfMiddleTwo) {
    auto result = callMedian({Value(1.0), Value(2.0), Value(3.0), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.5, result.asNumber());
}

TEST_F(MedianFunctionTest, FiveNumbers_ReturnsMiddleValue) {
    auto result = callMedian({Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(MedianFunctionTest, NegativeNumbers_ReturnsCorrectMedian) {
    auto result = callMedian({Value(-10.0), Value(-5.0), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-5.0, result.asNumber());
}

TEST_F(MedianFunctionTest, MixedPositiveNegative_ReturnsCorrectMedian) {
    auto result = callMedian({Value(10.0), Value(-5.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(MedianFunctionTest, WithBooleans_ConvertsBooleans) {
    auto result = callMedian({Value(1.0), Value(true), Value(false), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());  // [0, 1, 1, 4] -> median is 1
}

TEST_F(MedianFunctionTest, WithTextValues_IgnoresText) {
    auto result = callMedian({Value(1.0), Value("hello"), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // [1, 5] -> median is 3
}

TEST_F(MedianFunctionTest, WithEmptyValues_IgnoresEmpty) {
    auto result = callMedian({Value(2.0), Value::empty(), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // [2, 4] -> median is 3
}

TEST_F(MedianFunctionTest, OnlyTextValues_ReturnsError) {
    auto result = callMedian({Value("hello"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(MedianFunctionTest, OnlyEmptyValues_ReturnsError) {
    auto result = callMedian({Value::empty(), Value::empty()});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(MedianFunctionTest, WithErrorValue_PropagatesError) {
    auto result = callMedian({Value(1.0), Value::error(ErrorType::REF_ERROR), Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(MedianFunctionTest, DecimalNumbers_ReturnsCorrectMedian) {
    auto result = callMedian({Value(1.5), Value(2.25), Value(0.25)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.5, result.asNumber());
}

TEST_F(MedianFunctionTest, LargeNumbers_HandlesCorrectly) {
    auto result = callMedian({Value(1000000.0), Value(2000000.0), Value(3000000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2000000.0, result.asNumber());
}

TEST_F(MedianFunctionTest, ZeroValues_HandlesCorrectly) {
    auto result = callMedian({Value(0.0), Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(MedianFunctionTest, UnsortedNumbers_ReturnsCorrectMedian) {
    auto result = callMedian({Value(5.0), Value(1.0), Value(3.0), Value(2.0), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(MedianFunctionTest, DuplicateValues_ReturnsCorrectMedian) {
    auto result = callMedian({Value(1.0), Value(2.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}