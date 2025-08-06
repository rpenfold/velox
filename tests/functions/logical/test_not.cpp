#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class NotFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callNot(const std::vector<Value>& args) {
        return builtin::not_function(args, context);
    }
};

TEST_F(NotFunctionTest, NoArguments_ReturnsError) {
    auto result = callNot({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(NotFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callNot({Value(true), Value(false)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(NotFunctionTest, TrueArgument_ReturnsFalse) {
    auto result = callNot({Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(NotFunctionTest, FalseArgument_ReturnsTrue) {
    auto result = callNot({Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(NotFunctionTest, NonZeroNumber_ReturnsFalse) {
    auto result = callNot({Value(1.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(NotFunctionTest, ZeroNumber_ReturnsTrue) {
    auto result = callNot({Value(0.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(NotFunctionTest, NegativeNumber_ReturnsFalse) {
    auto result = callNot({Value(-5.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(NotFunctionTest, NonEmptyText_ReturnsFalse) {
    auto result = callNot({Value("hello")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(NotFunctionTest, EmptyText_ReturnsTrue) {
    auto result = callNot({Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(NotFunctionTest, WhitespaceText_ReturnsFalse) {
    auto result = callNot({Value(" ")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(NotFunctionTest, EmptyValue_ReturnsTrue) {
    auto result = callNot({Value::empty()});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(NotFunctionTest, ErrorArgument_PropagatesError) {
    auto result = callNot({Value::error(ErrorType::VALUE_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(NotFunctionTest, BooleanConversion_WorksCorrectly) {
    // Test that numbers convert to boolean correctly
    auto result = callNot({Value(1.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(NotFunctionTest, TextConversion_WorksCorrectly) {
    // Test that text converts to boolean correctly
    auto result = callNot({Value("hello")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(NotFunctionTest, DoubleNegation_ReturnsOriginal) {
    auto result1 = callNot({Value(true)});
    auto result2 = callNot({result1});

    EXPECT_TRUE(result2.isBoolean());
    EXPECT_TRUE(result2.asBoolean());
}

TEST_F(NotFunctionTest, LargeNumber_ReturnsFalse) {
    auto result = callNot({Value(1000000.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(NotFunctionTest, SmallNumber_ReturnsFalse) {
    auto result = callNot({Value(0.000001)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}