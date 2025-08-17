#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class AndFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callAnd(const std::vector<Value>& args) {
        return builtin::and_function(args, context);
    }
};

TEST_F(AndFunctionTest, NoArguments_ReturnsError) {
    auto result = callAnd({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(AndFunctionTest, SingleTrueArgument_ReturnsTrue) {
    auto result = callAnd({Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(AndFunctionTest, SingleFalseArgument_ReturnsFalse) {
    auto result = callAnd({Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(AndFunctionTest, AllTrueArguments_ReturnsTrue) {
    auto result = callAnd({Value(true), Value(true), Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(AndFunctionTest, OneFalseArgument_ReturnsFalse) {
    auto result = callAnd({Value(true), Value(false), Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(AndFunctionTest, AllFalseArguments_ReturnsFalse) {
    auto result = callAnd({Value(false), Value(false), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(AndFunctionTest, NonZeroNumbers_EvaluateToTrue) {
    auto result = callAnd({Value(1.0), Value(5.0), Value(-3.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(AndFunctionTest, ZeroNumber_EvaluatesToFalse) {
    auto result = callAnd({Value(1.0), Value(0.0), Value(5.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(AndFunctionTest, NonEmptyText_EvaluatesToTrue) {
    auto result = callAnd({Value("hello"), Value("world"), Value("test")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(AndFunctionTest, EmptyText_EvaluatesToFalse) {
    auto result = callAnd({Value("hello"), Value(""), Value("world")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(AndFunctionTest, MixedTypes_HandlesCorrectly) {
    auto result = callAnd({Value(true), Value(1.0), Value("hello")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(AndFunctionTest, EmptyValues_EvaluateToFalse) {
    auto result = callAnd({Value(true), Value::empty(), Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(AndFunctionTest, ErrorInArguments_PropagatesError) {
    auto result = callAnd({Value(true), Value::error(ErrorType::VALUE_ERROR), Value(true)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(AndFunctionTest, BooleanConversion_WorksCorrectly) {
    // Test that numbers convert to boolean correctly
    auto result = callAnd({Value(1.0), Value(true), Value(5.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(AndFunctionTest, TextConversion_WorksCorrectly) {
    // Test that text converts to boolean correctly
    auto result = callAnd({Value("hello"), Value(true), Value("world")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(AndFunctionTest, WhitespaceText_EvaluatesToTrue) {
    auto result = callAnd({Value(" "), Value("hello"), Value("  ")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}