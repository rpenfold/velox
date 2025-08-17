#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class CountFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callCount(const std::vector<Value>& args) {
        return builtin::count(args, context);
    }

    Value callCountA(const std::vector<Value>& args) {
        return builtin::counta(args, context);
    }
};

// COUNT Function Tests
TEST_F(CountFunctionTest, Count_EmptyArguments_ReturnsZero) {
    auto result = callCount({});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(CountFunctionTest, Count_OnlyNumbers_CountsAll) {
    auto result = callCount({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(CountFunctionTest, Count_MixedTypes_CountsOnlyNumbers) {
    auto result = callCount({Value(1.0), Value("hello"), Value(true), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(CountFunctionTest, Count_WithBooleans_IgnoresBooleans) {
    auto result = callCount({Value(1.0), Value(true), Value(false), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(CountFunctionTest, Count_WithText_IgnoresText) {
    auto result = callCount({Value("hello"), Value("world"), Value("123")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(CountFunctionTest, Count_WithEmptyValues_IgnoresEmpty) {
    auto result = callCount({Value(1.0), Value::empty(), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(CountFunctionTest, Count_WithNegativeNumbers_CountsNegatives) {
    auto result = callCount({Value(-1.0), Value(0.0), Value(-2.5)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(CountFunctionTest, Count_WithErrorValue_PropagatesError) {
    auto result = callCount({Value(1.0), Value::error(ErrorType::VALUE_ERROR), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// COUNTA Function Tests
TEST_F(CountFunctionTest, CountA_EmptyArguments_ReturnsZero) {
    auto result = callCountA({});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(CountFunctionTest, CountA_OnlyNumbers_CountsAll) {
    auto result = callCountA({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(CountFunctionTest, CountA_MixedTypes_CountsAllNonEmpty) {
    auto result = callCountA({Value(1.0), Value("hello"), Value(true), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(CountFunctionTest, CountA_WithBooleans_CountsBooleans) {
    auto result = callCountA({Value(true), Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(CountFunctionTest, CountA_WithText_CountsText) {
    auto result = callCountA({Value("hello"), Value("world"), Value("")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(CountFunctionTest, CountA_WithEmptyValues_IgnoresEmpty) {
    auto result = callCountA({Value(1.0), Value::empty(), Value("hello")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(CountFunctionTest, CountA_OnlyEmptyValues_ReturnsZero) {
    auto result = callCountA({Value::empty(), Value::empty()});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(CountFunctionTest, CountA_WithErrors_CountsErrors) {
    auto result = callCountA({Value(1.0), Value::error(ErrorType::DIV_ZERO), Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(CountFunctionTest, CountA_WithZeroString_CountsZeroString) {
    auto result = callCountA({Value("0"), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(CountFunctionTest, CountA_WithEmptyString_CountsEmptyString) {
    auto result = callCountA({Value(""), Value(" ")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}