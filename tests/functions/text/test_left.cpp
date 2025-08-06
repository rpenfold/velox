#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class LeftFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callLeft(const std::vector<Value>& args) {
        return builtin::left(args, context);
    }
};

TEST_F(LeftFunctionTest, NoArguments_ReturnsError) {
    auto result = callLeft({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LeftFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callLeft({Value("hello"), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LeftFunctionTest, SingleArgument_ReturnsFirstCharacter) {
    auto result = callLeft({Value("hello")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("h", result.asText());
}

TEST_F(LeftFunctionTest, EmptyString_ReturnsEmptyString) {
    auto result = callLeft({Value("")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(LeftFunctionTest, SingleCharacter_ReturnsSameCharacter) {
    auto result = callLeft({Value("a")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("a", result.asText());
}

TEST_F(LeftFunctionTest, TwoArguments_ReturnsSpecifiedCharacters) {
    auto result = callLeft({Value("hello"), Value(3.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hel", result.asText());
}

TEST_F(LeftFunctionTest, NumCharsGreaterThanLength_ReturnsEntireString) {
    auto result = callLeft({Value("hello"), Value(10.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(LeftFunctionTest, NumCharsZero_ReturnsEmptyString) {
    auto result = callLeft({Value("hello"), Value(0.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(LeftFunctionTest, NegativeNumChars_ReturnsEmptyString) {
    auto result = callLeft({Value("hello"), Value(-1.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(LeftFunctionTest, NonNumericSecondArgument_ReturnsError) {
    auto result = callLeft({Value("hello"), Value("abc")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LeftFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callLeft({Value(123.45), Value(2.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("12", result.asText());
}

TEST_F(LeftFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callLeft({Value(true), Value(1.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("T", result.asText());
}

TEST_F(LeftFunctionTest, TextWithSpaces_HandlesSpaces) {
    auto result = callLeft({Value("hello world"), Value(5.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(LeftFunctionTest, TextWithSpecialCharacters_HandlesSpecialChars) {
    auto result = callLeft({Value("hello!@#"), Value(6.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello!", result.asText());
}

TEST_F(LeftFunctionTest, TextWithNumbers_HandlesNumbers) {
    auto result = callLeft({Value("abc123"), Value(4.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("abc1", result.asText());
}

TEST_F(LeftFunctionTest, TextWithNewlines_HandlesNewlines) {
    auto result = callLeft({Value("line1\nline2"), Value(6.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("line1\n", result.asText());
}

TEST_F(LeftFunctionTest, ErrorInput_PropagatesError) {
    auto result = callLeft({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(LeftFunctionTest, ErrorInSecondArgument_PropagatesError) {
    auto result = callLeft({Value("hello"), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}