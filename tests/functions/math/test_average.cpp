#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class AverageFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callAverage(const std::vector<Value>& args) {
        return builtin::average(args, context);
    }
};

TEST_F(AverageFunctionTest, EmptyArguments_ReturnsError) {
    auto result = callAverage({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(AverageFunctionTest, SingleNumber_ReturnsSameNumber) {
    auto result = callAverage({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(AverageFunctionTest, MultipleNumbers_ReturnsAverage) {
    auto result = callAverage({Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(AverageFunctionTest, NegativeNumbers_ReturnsCorrectAverage) {
    auto result = callAverage({Value(-10.0), Value(-5.0), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-16.0 / 3.0, result.asNumber());
}

TEST_F(AverageFunctionTest, MixedPositiveNegative_ReturnsCorrectAverage) {
    auto result = callAverage({Value(10.0), Value(-5.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(AverageFunctionTest, WithBooleans_ConvertsBooleans) {
    auto result = callAverage({Value(1.0), Value(true), Value(false), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.5, result.asNumber());  // (1 + 1 + 0 + 4) / 4
}

TEST_F(AverageFunctionTest, WithTextValues_IgnoresText) {
    auto result = callAverage({Value(1.0), Value("hello"), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // (1 + 5) / 2
}

TEST_F(AverageFunctionTest, WithEmptyValues_IgnoresEmpty) {
    auto result = callAverage({Value(2.0), Value::empty(), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // (2 + 4) / 2
}

TEST_F(AverageFunctionTest, OnlyTextValues_ReturnsError) {
    auto result = callAverage({Value("hello"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(AverageFunctionTest, OnlyEmptyValues_ReturnsError) {
    auto result = callAverage({Value::empty(), Value::empty()});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(AverageFunctionTest, WithErrorValue_PropagatesError) {
    auto result = callAverage({Value(1.0), Value::error(ErrorType::REF_ERROR), Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(AverageFunctionTest, DecimalNumbers_ReturnsCorrectAverage) {
    auto result = callAverage({Value(1.5), Value(2.25), Value(0.25)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0 / 3.0, result.asNumber());
}

TEST_F(AverageFunctionTest, LargeNumbers_HandlesCorrectly) {
    auto result = callAverage({Value(1000000.0), Value(2000000.0), Value(3000000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2000000.0, result.asNumber());
}

TEST_F(AverageFunctionTest, ZeroValues_HandlesCorrectly) {
    auto result = callAverage({Value(0.0), Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}