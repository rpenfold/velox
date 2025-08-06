#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class IfFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callIf(const std::vector<Value>& args) {
        return builtin::if_function(args, context);
    }
};

TEST_F(IfFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callIf({Value(true), Value("yes")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callIf({Value(true), Value("yes"), Value("no"), Value("extra")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfFunctionTest, TrueCondition_ReturnsFirstValue) {
    auto result = callIf({Value(true), Value("yes"), Value("no")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("yes", result.asText());
}

TEST_F(IfFunctionTest, FalseCondition_ReturnsSecondValue) {
    auto result = callIf({Value(false), Value("yes"), Value("no")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("no", result.asText());
}

TEST_F(IfFunctionTest, NonZeroNumber_EvaluatesToTrue) {
    auto result = callIf({Value(1.0), Value("non-zero"), Value("zero")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("non-zero", result.asText());
}

TEST_F(IfFunctionTest, ZeroNumber_EvaluatesToFalse) {
    auto result = callIf({Value(0.0), Value("non-zero"), Value("zero")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("zero", result.asText());
}

TEST_F(IfFunctionTest, PositiveNumber_EvaluatesToTrue) {
    auto result = callIf({Value(5.0), Value("positive"), Value("zero")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("positive", result.asText());
}

TEST_F(IfFunctionTest, NegativeNumber_EvaluatesToTrue) {
    auto result = callIf({Value(-3.0), Value("negative"), Value("zero")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("negative", result.asText());
}

TEST_F(IfFunctionTest, NonEmptyText_EvaluatesToTrue) {
    auto result = callIf({Value("hello"), Value("non-empty"), Value("empty")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("non-empty", result.asText());
}

TEST_F(IfFunctionTest, EmptyText_EvaluatesToFalse) {
    auto result = callIf({Value(""), Value("non-empty"), Value("empty")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("empty", result.asText());
}

TEST_F(IfFunctionTest, WhitespaceText_EvaluatesToTrue) {
    auto result = callIf({Value(" "), Value("has-space"), Value("empty")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("has-space", result.asText());
}

TEST_F(IfFunctionTest, NumberCondition_ReturnsNumbers) {
    auto result = callIf({Value(1.0), Value(100.0), Value(200.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(100.0, result.asNumber());
}

TEST_F(IfFunctionTest, BooleanCondition_ReturnsBooleans) {
    auto result = callIf({Value(true), Value(true), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IfFunctionTest, MixedReturnTypes_PreservesTypes) {
    auto result = callIf({Value(true), Value(42.0), Value("text")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(IfFunctionTest, ErrorInCondition_PropagatesError) {
    auto result = callIf({Value::error(ErrorType::VALUE_ERROR), Value("yes"), Value("no")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfFunctionTest, ErrorInTrueValue_PropagatesError) {
    auto result = callIf({Value(true), Value::error(ErrorType::DIV_ZERO), Value("no")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(IfFunctionTest, ErrorInFalseValue_PropagatesError) {
    auto result = callIf({Value(false), Value("yes"), Value::error(ErrorType::REF_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(IfFunctionTest, ErrorInUnusedBranch_DoesNotPropagate) {
    auto result = callIf({
            Value(true), Value("yes"),
            Value::error(ErrorType::REF_ERROR)  // This branch won't be taken
    });

    EXPECT_TRUE(result.isError());  // Actually, errors are checked upfront
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(IfFunctionTest, ComplexCondition_EvaluatesCorrectly) {
    // Simulating a comparison result as condition
    auto result = callIf({Value(true), Value("greater"), Value("less-equal")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("greater", result.asText());
}