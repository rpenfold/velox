#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class UpperFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callUpper(const std::vector<Value>& args) {
        return builtin::upper(args, context);
    }
};

TEST_F(UpperFunctionTest, NoArguments_ReturnsError) {
    auto result = callUpper({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(UpperFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callUpper({Value("hello"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(UpperFunctionTest, SimpleText_ConvertsToUppercase) {
    auto result = callUpper({Value("hello")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("HELLO", result.asText());
}

TEST_F(UpperFunctionTest, AlreadyUppercase_ReturnsSame) {
    auto result = callUpper({Value("HELLO")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("HELLO", result.asText());
}

TEST_F(UpperFunctionTest, MixedCase_ConvertsToUppercase) {
    auto result = callUpper({Value("HeLLo")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("HELLO", result.asText());
}

TEST_F(UpperFunctionTest, EmptyString_ReturnsEmptyString) {
    auto result = callUpper({Value("")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(UpperFunctionTest, SingleCharacter_ConvertsToUppercase) {
    auto result = callUpper({Value("a")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(UpperFunctionTest, SingleUppercaseCharacter_ReturnsSame) {
    auto result = callUpper({Value("A")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(UpperFunctionTest, TextWithSpaces_HandlesSpaces) {
    auto result = callUpper({Value("hello world")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("HELLO WORLD", result.asText());
}

TEST_F(UpperFunctionTest, TextWithSpecialCharacters_HandlesSpecialChars) {
    auto result = callUpper({Value("hello!@#")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("HELLO!@#", result.asText());
}

TEST_F(UpperFunctionTest, TextWithNumbers_HandlesNumbers) {
    auto result = callUpper({Value("abc123")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("ABC123", result.asText());
}

TEST_F(UpperFunctionTest, TextWithNewlines_HandlesNewlines) {
    auto result = callUpper({Value("line1\nline2")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("LINE1\nLINE2", result.asText());
}

TEST_F(UpperFunctionTest, TextWithTabs_HandlesTabs) {
    auto result = callUpper({Value("hello\tworld")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("HELLO\tWORLD", result.asText());
}

TEST_F(UpperFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callUpper({Value(123.45)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("123.45", result.asText());
}

TEST_F(UpperFunctionTest, NegativeNumberInput_ConvertsToTextFirst) {
    auto result = callUpper({Value(-123.45)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("-123.45", result.asText());
}

TEST_F(UpperFunctionTest, TrueBooleanInput_ConvertsToTextFirst) {
    auto result = callUpper({Value(true)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("TRUE", result.asText());
}

TEST_F(UpperFunctionTest, FalseBooleanInput_ConvertsToTextFirst) {
    auto result = callUpper({Value(false)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("FALSE", result.asText());
}

TEST_F(UpperFunctionTest, ErrorInput_PropagatesError) {
    auto result = callUpper({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(UpperFunctionTest, VeryLongString_HandlesLongString) {
    std::string longString(1000, 'a');
    auto result = callUpper({Value(longString)});

    EXPECT_TRUE(result.isText());
    std::string expected(1000, 'A');
    EXPECT_EQ(expected, result.asText());
}

TEST_F(UpperFunctionTest, TextWithAccentedCharacters_HandlesAccents) {
    auto result = callUpper({Value("café")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("CAFÉ", result.asText());
}

TEST_F(UpperFunctionTest, TextWithUnicode_HandlesUnicode) {
    auto result = callUpper({Value("αβγ")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("ΑΒΓ", result.asText());
}