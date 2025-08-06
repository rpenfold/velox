#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class ProperFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callProper(const std::vector<Value>& args) {
        return builtin::proper(args, context);
    }
};

TEST_F(ProperFunctionTest, NoArguments_ReturnsError) {
    auto result = callProper({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ProperFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callProper({Value("hello"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ProperFunctionTest, SimpleText_CapitalizesFirstLetter) {
    auto result = callProper({Value("hello")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello", result.asText());
}

TEST_F(ProperFunctionTest, AlreadyProper_ReturnsSame) {
    auto result = callProper({Value("Hello")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello", result.asText());
}

TEST_F(ProperFunctionTest, AllUppercase_ConvertsToProper) {
    auto result = callProper({Value("HELLO")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello", result.asText());
}

TEST_F(ProperFunctionTest, MixedCase_ConvertsToProper) {
    auto result = callProper({Value("hElLo")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello", result.asText());
}

TEST_F(ProperFunctionTest, EmptyString_ReturnsEmptyString) {
    auto result = callProper({Value("")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(ProperFunctionTest, SingleCharacter_Capitalizes) {
    auto result = callProper({Value("a")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(ProperFunctionTest, SingleUppercaseCharacter_ReturnsSame) {
    auto result = callProper({Value("A")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(ProperFunctionTest, TextWithSpaces_CapitalizesEachWord) {
    auto result = callProper({Value("hello world")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello World", result.asText());
}

TEST_F(ProperFunctionTest, TextWithMultipleSpaces_HandlesMultipleSpaces) {
    auto result = callProper({Value("hello  world")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello  World", result.asText());
}

TEST_F(ProperFunctionTest, TextWithLeadingSpaces_HandlesLeadingSpaces) {
    auto result = callProper({Value("  hello world")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("  Hello World", result.asText());
}

TEST_F(ProperFunctionTest, TextWithTrailingSpaces_HandlesTrailingSpaces) {
    auto result = callProper({Value("hello world  ")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello World  ", result.asText());
}

TEST_F(ProperFunctionTest, TextWithSpecialCharacters_HandlesSpecialChars) {
    auto result = callProper({Value("hello!@#world")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello!@#world", result.asText());
}

TEST_F(ProperFunctionTest, TextWithNumbers_HandlesNumbers) {
    auto result = callProper({Value("abc123def")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Abc123def", result.asText());
}

TEST_F(ProperFunctionTest, TextWithHyphens_HandlesHyphens) {
    auto result = callProper({Value("hello-world")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello-world", result.asText());
}

TEST_F(ProperFunctionTest, TextWithUnderscores_HandlesUnderscores) {
    auto result = callProper({Value("hello_world")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello_world", result.asText());
}

TEST_F(ProperFunctionTest, TextWithNewlines_HandlesNewlines) {
    auto result = callProper({Value("hello\nworld")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello\nWorld", result.asText());
}

TEST_F(ProperFunctionTest, TextWithTabs_HandlesTabs) {
    auto result = callProper({Value("hello\tworld")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello\tWorld", result.asText());
}

TEST_F(ProperFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callProper({Value(123.45)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("123.45", result.asText());
}

TEST_F(ProperFunctionTest, NegativeNumberInput_ConvertsToTextFirst) {
    auto result = callProper({Value(-123.45)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("-123.45", result.asText());
}

TEST_F(ProperFunctionTest, TrueBooleanInput_ConvertsToTextFirst) {
    auto result = callProper({Value(true)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("True", result.asText());
}

TEST_F(ProperFunctionTest, FalseBooleanInput_ConvertsToTextFirst) {
    auto result = callProper({Value(false)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("False", result.asText());
}

TEST_F(ProperFunctionTest, ErrorInput_PropagatesError) {
    auto result = callProper({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ProperFunctionTest, VeryLongString_HandlesLongString) {
    std::string longString(1000, 'a');
    auto result = callProper({Value(longString)});

    EXPECT_TRUE(result.isText());
    std::string expected = "A" + std::string(999, 'a');
    EXPECT_EQ(expected, result.asText());
}

TEST_F(ProperFunctionTest, TextWithAccentedCharacters_HandlesAccents) {
    auto result = callProper({Value("café")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Café", result.asText());
}

TEST_F(ProperFunctionTest, TextWithUnicode_HandlesUnicode) {
    auto result = callProper({Value("αβγ")});

    EXPECT_TRUE(result.isText());
    // Note: std::toupper/std::tolower have limited Unicode support
    // This test documents the current behavior
    EXPECT_EQ("αβγ", result.asText());
}

TEST_F(ProperFunctionTest, MultipleWordsWithVariousSeparators) {
    auto result = callProper({Value("hello-world_test 123")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello-world_test 123", result.asText());
}