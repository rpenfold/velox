#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class ReplaceFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callReplace(const std::vector<Value>& args) {
        return builtin::replace(args, context);
    }
};

TEST_F(ReplaceFunctionTest, NoArguments_ReturnsError) {
    auto result = callReplace({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ReplaceFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callReplace({Value("hello"), Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ReplaceFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callReplace({Value("hello"), Value(1.0), Value(2.0), Value("world"), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ReplaceFunctionTest, NonNumericStartNum_ReturnsError) {
    auto result = callReplace({Value("hello"), Value("abc"), Value(2.0), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ReplaceFunctionTest, NonNumericNumChars_ReturnsError) {
    auto result = callReplace({Value("hello"), Value(1.0), Value("abc"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ReplaceFunctionTest, StartNumLessThanOne_ReturnsError) {
    auto result = callReplace({Value("hello"), Value(0.0), Value(2.0), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ReplaceFunctionTest, NumCharsNegative_ReturnsError) {
    auto result = callReplace({Value("hello"), Value(1.0), Value(-1.0), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ReplaceFunctionTest, BasicReplace_ReplacesCorrectly) {
    auto result = callReplace({Value("hello world"), Value(7.0), Value(5.0), Value("earth")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello earth", result.asText());
}

TEST_F(ReplaceFunctionTest, ReplaceAtBeginning_ReplacesCorrectly) {
    auto result = callReplace({Value("hello world"), Value(1.0), Value(5.0), Value("goodbye")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("goodbye world", result.asText());
}

TEST_F(ReplaceFunctionTest, ReplaceAtEnd_ReplacesCorrectly) {
    auto result = callReplace({Value("hello world"), Value(7.0), Value(5.0), Value("universe")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello universe", result.asText());
}

TEST_F(ReplaceFunctionTest, ReplaceWithEmptyString_RemovesCharacters) {
    auto result = callReplace({Value("hello world"), Value(7.0), Value(5.0), Value("")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello ", result.asText());
}

TEST_F(ReplaceFunctionTest, ReplaceWithLongerString_ExpandsText) {
    auto result = callReplace({Value("hello world"), Value(7.0), Value(5.0), Value("beautiful earth")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello beautiful earth", result.asText());
}

TEST_F(ReplaceFunctionTest, StartPositionBeyondText_AppendsNewText) {
    auto result = callReplace({Value("hello"), Value(10.0), Value(5.0), Value("world")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("helloworld", result.asText());
}

TEST_F(ReplaceFunctionTest, NumCharsExceedsAvailable_ReplacesAllRemaining) {
    auto result = callReplace({Value("hello world"), Value(7.0), Value(10.0), Value("earth")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello earth", result.asText());
}

TEST_F(ReplaceFunctionTest, ReplaceSingleCharacter_ReplacesCorrectly) {
    auto result = callReplace({Value("hello world"), Value(6.0), Value(1.0), Value("X")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("helloXworld", result.asText());
}

TEST_F(ReplaceFunctionTest, ReplaceWithSpecialCharacters_HandlesSpecialChars) {
    auto result = callReplace({Value("hello world"), Value(7.0), Value(5.0), Value("!@#$%")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello !@#$%", result.asText());
}

TEST_F(ReplaceFunctionTest, ReplaceWithNumbers_HandlesNumbers) {
    auto result = callReplace({Value("hello world"), Value(7.0), Value(5.0), Value("12345")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello 12345", result.asText());
}

TEST_F(ReplaceFunctionTest, ReplaceWithSpaces_HandlesSpaces) {
    auto result = callReplace({Value("hello world"), Value(7.0), Value(5.0), Value("   ")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello    ", result.asText());
}

TEST_F(ReplaceFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callReplace({Value(123.45), Value(1.0), Value(3.0), Value("abc")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("abc.45", result.asText());
}

TEST_F(ReplaceFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callReplace({Value(true), Value(1.0), Value(1.0), Value("FALSE")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("FALSERUE", result.asText());
}

TEST_F(ReplaceFunctionTest, ErrorInput_PropagatesError) {
    auto result = callReplace({Value::error(ErrorType::DIV_ZERO), Value(1.0), Value(2.0), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ReplaceFunctionTest, ErrorInSecondArgument_PropagatesError) {
    auto result = callReplace({Value("hello"), Value::error(ErrorType::DIV_ZERO), Value(2.0), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ReplaceFunctionTest, ErrorInThirdArgument_PropagatesError) {
    auto result = callReplace({Value("hello"), Value(1.0), Value::error(ErrorType::DIV_ZERO), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ReplaceFunctionTest, ErrorInFourthArgument_PropagatesError) {
    auto result = callReplace({Value("hello"), Value(1.0), Value(2.0), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}