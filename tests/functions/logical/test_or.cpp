#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class OrFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callOr(const std::vector<Value>& args) {
        return builtin::or_function(args, context);
    }
};

TEST_F(OrFunctionTest, NoArguments_ReturnsError) {
    auto result = callOr({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(OrFunctionTest, SingleTrueArgument_ReturnsTrue) {
    auto result = callOr({Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(OrFunctionTest, SingleFalseArgument_ReturnsFalse) {
    auto result = callOr({Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(OrFunctionTest, AllTrueArguments_ReturnsTrue) {
    auto result = callOr({Value(true), Value(true), Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(OrFunctionTest, OneTrueArgument_ReturnsTrue) {
    auto result = callOr({Value(false), Value(true), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(OrFunctionTest, AllFalseArguments_ReturnsFalse) {
    auto result = callOr({Value(false), Value(false), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(OrFunctionTest, NonZeroNumbers_EvaluateToTrue) {
    auto result = callOr({Value(0.0), Value(5.0), Value(0.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(OrFunctionTest, AllZeroNumbers_EvaluateToFalse) {
    auto result = callOr({Value(0.0), Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(OrFunctionTest, NonEmptyText_EvaluatesToTrue) {
    auto result = callOr({Value(""), Value("hello"), Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(OrFunctionTest, AllEmptyText_EvaluatesToFalse) {
    auto result = callOr({Value(""), Value(""), Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(OrFunctionTest, MixedTypes_HandlesCorrectly) {
    auto result = callOr({Value(false), Value(1.0), Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(OrFunctionTest, EmptyValues_EvaluateToFalse) {
    auto result = callOr({Value::empty(), Value(false), Value::empty()});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(OrFunctionTest, ErrorInArguments_PropagatesError) {
    auto result = callOr({Value(false), Value::error(ErrorType::VALUE_ERROR), Value(false)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(OrFunctionTest, BooleanConversion_WorksCorrectly) {
    // Test that numbers convert to boolean correctly
    auto result = callOr({Value(0.0), Value(true), Value(0.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(OrFunctionTest, TextConversion_WorksCorrectly) {
    // Test that text converts to boolean correctly
    auto result = callOr({Value(""), Value("hello"), Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(OrFunctionTest, WhitespaceText_EvaluatesToTrue) {
    auto result = callOr({Value(""), Value(" "), Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(OrFunctionTest, FirstTrueArgument_ReturnsTrue) {
    auto result = callOr({Value(true), Value(false), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(OrFunctionTest, LastTrueArgument_ReturnsTrue) {
    auto result = callOr({Value(false), Value(false), Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}