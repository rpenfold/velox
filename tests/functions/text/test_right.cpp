#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class RightFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callRight(const std::vector<Value>& args) {
        return builtin::right(args, context);
    }
};

TEST_F(RightFunctionTest, NoArguments_ReturnsError) {
    auto result = callRight({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RightFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callRight({Value("hello"), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RightFunctionTest, SingleArgument_ReturnsLastCharacter) {
    auto result = callRight({Value("hello")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("o", result.asText());
}

TEST_F(RightFunctionTest, EmptyString_ReturnsEmptyString) {
    auto result = callRight({Value("")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(RightFunctionTest, SingleCharacter_ReturnsSameCharacter) {
    auto result = callRight({Value("a")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("a", result.asText());
}

TEST_F(RightFunctionTest, TwoArguments_ReturnsSpecifiedCharacters) {
    auto result = callRight({Value("hello"), Value(3.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("llo", result.asText());
}

TEST_F(RightFunctionTest, NumCharsGreaterThanLength_ReturnsEntireString) {
    auto result = callRight({Value("hello"), Value(10.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(RightFunctionTest, NumCharsZero_ReturnsEmptyString) {
    auto result = callRight({Value("hello"), Value(0.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(RightFunctionTest, NegativeNumChars_ReturnsEmptyString) {
    auto result = callRight({Value("hello"), Value(-1.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(RightFunctionTest, NonNumericSecondArgument_ReturnsError) {
    auto result = callRight({Value("hello"), Value("abc")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RightFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callRight({Value(123.45), Value(2.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("45", result.asText());
}

TEST_F(RightFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callRight({Value(true), Value(1.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("E", result.asText());
}

TEST_F(RightFunctionTest, TextWithSpaces_HandlesSpaces) {
    auto result = callRight({Value("hello world"), Value(5.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("world", result.asText());
}

TEST_F(RightFunctionTest, TextWithSpecialCharacters_HandlesSpecialChars) {
    auto result = callRight({Value("hello!@#"), Value(3.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("!@#", result.asText());
}

TEST_F(RightFunctionTest, TextWithNumbers_HandlesNumbers) {
    auto result = callRight({Value("abc123"), Value(3.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("123", result.asText());
}

TEST_F(RightFunctionTest, TextWithNewlines_HandlesNewlines) {
    auto result = callRight({Value("line1\nline2"), Value(5.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("line2", result.asText());
}

TEST_F(RightFunctionTest, ErrorInput_PropagatesError) {
    auto result = callRight({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(RightFunctionTest, ErrorInSecondArgument_PropagatesError) {
    auto result = callRight({Value("hello"), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}