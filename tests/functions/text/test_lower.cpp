#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class LowerFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callLower(const std::vector<Value>& args) {
        return builtin::lower(args, context);
    }
};

TEST_F(LowerFunctionTest, NoArguments_ReturnsError) {
    auto result = callLower({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LowerFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callLower({Value("HELLO"), Value("WORLD")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LowerFunctionTest, SimpleText_ConvertsToLowercase) {
    auto result = callLower({Value("HELLO")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(LowerFunctionTest, AlreadyLowercase_ReturnsSame) {
    auto result = callLower({Value("hello")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(LowerFunctionTest, MixedCase_ConvertsToLowercase) {
    auto result = callLower({Value("HeLLo")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(LowerFunctionTest, EmptyString_ReturnsEmptyString) {
    auto result = callLower({Value("")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(LowerFunctionTest, SingleCharacter_ConvertsToLowercase) {
    auto result = callLower({Value("A")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("a", result.asText());
}

TEST_F(LowerFunctionTest, SingleLowercaseCharacter_ReturnsSame) {
    auto result = callLower({Value("a")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("a", result.asText());
}

TEST_F(LowerFunctionTest, TextWithSpaces_HandlesSpaces) {
    auto result = callLower({Value("HELLO WORLD")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello world", result.asText());
}

TEST_F(LowerFunctionTest, TextWithSpecialCharacters_HandlesSpecialChars) {
    auto result = callLower({Value("HELLO!@#")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello!@#", result.asText());
}

TEST_F(LowerFunctionTest, TextWithNumbers_HandlesNumbers) {
    auto result = callLower({Value("ABC123")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("abc123", result.asText());
}

TEST_F(LowerFunctionTest, TextWithNewlines_HandlesNewlines) {
    auto result = callLower({Value("LINE1\nLINE2")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("line1\nline2", result.asText());
}

TEST_F(LowerFunctionTest, TextWithTabs_HandlesTabs) {
    auto result = callLower({Value("HELLO\tWORLD")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello\tworld", result.asText());
}

TEST_F(LowerFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callLower({Value(123.45)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("123.45", result.asText());
}

TEST_F(LowerFunctionTest, NegativeNumberInput_ConvertsToTextFirst) {
    auto result = callLower({Value(-123.45)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("-123.45", result.asText());
}

TEST_F(LowerFunctionTest, TrueBooleanInput_ConvertsToTextFirst) {
    auto result = callLower({Value(true)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("true", result.asText());
}

TEST_F(LowerFunctionTest, FalseBooleanInput_ConvertsToTextFirst) {
    auto result = callLower({Value(false)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("false", result.asText());
}

TEST_F(LowerFunctionTest, ErrorInput_PropagatesError) {
    auto result = callLower({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(LowerFunctionTest, VeryLongString_HandlesLongString) {
    std::string longString(1000, 'A');
    auto result = callLower({Value(longString)});

    EXPECT_TRUE(result.isText());
    std::string expected(1000, 'a');
    EXPECT_EQ(expected, result.asText());
}

TEST_F(LowerFunctionTest, TextWithAccentedCharacters_HandlesAccents) {
    auto result = callLower({Value("CAFÉ")});

    EXPECT_TRUE(result.isText());
    // Note: std::tolower has limited Unicode support
    // This test documents the current behavior
    EXPECT_EQ("cafÉ", result.asText());
}

TEST_F(LowerFunctionTest, TextWithUnicode_HandlesUnicode) {
    auto result = callLower({Value("ΑΒΓ")});

    EXPECT_TRUE(result.isText());
    // Note: std::tolower has limited Unicode support
    // This test documents the current behavior
    EXPECT_EQ("ΑΒΓ", result.asText());
}