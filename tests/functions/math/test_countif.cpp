#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class CountIfFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callCountIf(const std::vector<Value>& args) {
        return countif(args, context);
    }
};

// Argument validation tests
TEST_F(CountIfFunctionTest, NoArguments_ReturnsError) {
    auto result = callCountIf({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(CountIfFunctionTest, OneArgument_ReturnsError) {
    auto result = callCountIf({Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Basic equality tests
TEST_F(CountIfFunctionTest, ExactNumberMatch_CountsCorrectly) {
    auto result = callCountIf({Value(1.0), Value(2.0), Value(1.0), Value(3.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(CountIfFunctionTest, ExactTextMatch_CountsCorrectly) {
    auto result = callCountIf(
            {Value("apple"), Value("banana"), Value("apple"), Value("cherry"), Value("apple")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(CountIfFunctionTest, ExactBooleanMatch_CountsCorrectly) {
    auto result = callCountIf({Value(true), Value(false), Value(true), Value(true), Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(CountIfFunctionTest, NoMatches_ReturnsZero) {
    auto result = callCountIf({Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

// Comparison operator tests
TEST_F(CountIfFunctionTest, GreaterThan_CountsCorrectly) {
    auto result = callCountIf({Value(1.0), Value(5.0), Value(3.0), Value(7.0), Value(">3")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // 5 and 7 are > 3
}

TEST_F(CountIfFunctionTest, LessThan_CountsCorrectly) {
    auto result = callCountIf({Value(1.0), Value(5.0), Value(3.0), Value(7.0), Value("<5")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // 1 and 3 are < 5
}

TEST_F(CountIfFunctionTest, GreaterThanOrEqual_CountsCorrectly) {
    auto result = callCountIf({Value(1.0), Value(5.0), Value(3.0), Value(5.0), Value(">=5")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // both 5s are >= 5
}

TEST_F(CountIfFunctionTest, LessThanOrEqual_CountsCorrectly) {
    auto result = callCountIf({Value(1.0), Value(5.0), Value(3.0), Value(7.0), Value("<=3")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // 1 and 3 are <= 3
}

TEST_F(CountIfFunctionTest, NotEqual_CountsCorrectly) {
    auto result = callCountIf({Value(1.0), Value(5.0), Value(3.0), Value(5.0), Value("<>5")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // 1 and 3 are not equal to 5
}

TEST_F(CountIfFunctionTest, EqualOperator_CountsCorrectly) {
    auto result = callCountIf({Value(1.0), Value(5.0), Value(3.0), Value(5.0), Value("=5")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // both 5s equal 5
}

// Text comparison tests
TEST_F(CountIfFunctionTest, TextGreaterThan_CountsCorrectly) {
    auto result = callCountIf({Value("apple"), Value("banana"), Value("cherry"), Value(">banana")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());  // "cherry" > "banana"
}

TEST_F(CountIfFunctionTest, TextLessThan_CountsCorrectly) {
    auto result = callCountIf({Value("apple"), Value("banana"), Value("cherry"), Value("<cherry")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // "apple" and "banana" < "cherry"
}

// Wildcard tests
TEST_F(CountIfFunctionTest, StarWildcard_CountsCorrectly) {
    auto result =
            callCountIf({Value("apple"), Value("application"), Value("banana"), Value("app*")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // "apple" and "application" match "app*"
}

TEST_F(CountIfFunctionTest, QuestionWildcard_CountsCorrectly) {
    auto result =
            callCountIf({Value("cat"), Value("car"), Value("can"), Value("cap"), Value("ca?")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());  // all match "ca?"
}

TEST_F(CountIfFunctionTest, MixedWildcards_CountsCorrectly) {
    auto result = callCountIf({Value("test1"), Value("test22"), Value("test333"), Value("test*")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // all match "test*"
}

// Empty and null criteria tests
TEST_F(CountIfFunctionTest, EmptyCriteria_CountsEmptyValues) {
    auto result = callCountIf({Value(""), Value("hello"), Value(""), Value(""), Value("")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // three empty strings
}

// Mixed types tests
TEST_F(CountIfFunctionTest, MixedTypesWithNumberCriteria_CountsOnlyNumbers) {
    auto result = callCountIf({Value(5.0), Value("5"), Value(true), Value(5.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // only the actual numbers 5.0
}

TEST_F(CountIfFunctionTest, MixedTypesWithTextCriteria_CountsOnlyText) {
    auto result = callCountIf({Value(5.0), Value("5"), Value(true), Value("5"), Value("5"), Value("5")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // only the text "5"
}

// Error handling tests
TEST_F(CountIfFunctionTest, ErrorInValues_PropagatesError) {
    auto result = callCountIf(
            {Value(1.0), Value::error(ErrorType::DIV_ZERO), Value(3.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(CountIfFunctionTest, ErrorInCriteria_PropagatesError) {
    auto result = callCountIf(
            {Value(1.0), Value(2.0), Value(3.0), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

// Complex criteria tests
TEST_F(CountIfFunctionTest, NumericTextCriteria_ConvertsCorrectly) {
    auto result = callCountIf({Value(1.0), Value(5.0), Value(3.0), Value(7.0), Value("5")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());  // matches the numeric 5.0
}