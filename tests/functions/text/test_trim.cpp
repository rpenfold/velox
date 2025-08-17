#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class TrimFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callTrim(const std::vector<Value>& args) {
        return builtin::trim(args, context);
    }
};

TEST_F(TrimFunctionTest, NoArguments_ReturnsError) {
    auto result = callTrim({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TrimFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callTrim({Value("a"), Value("b")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TrimFunctionTest, TextWithLeadingSpaces_TrimsLeading) {
    auto result = callTrim({Value("  hello")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(TrimFunctionTest, TextWithTrailingSpaces_TrimsTrailing) {
    auto result = callTrim({Value("hello  ")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(TrimFunctionTest, TextWithBothSpaces_TrimsBoth) {
    auto result = callTrim({Value("  hello  ")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(TrimFunctionTest, TextWithInternalSpaces_PreservesInternal) {
    auto result = callTrim({Value("  hello world  ")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello world", result.asText());
}

TEST_F(TrimFunctionTest, TextWithoutSpaces_ReturnsUnchanged) {
    auto result = callTrim({Value("hello")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(TrimFunctionTest, EmptyString_ReturnsEmpty) {
    auto result = callTrim({Value("")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(TrimFunctionTest, OnlySpaces_ReturnsEmpty) {
    auto result = callTrim({Value("   ")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(TrimFunctionTest, MixedWhitespace_TrimsAllTypes) {
    auto result = callTrim({Value("\t\n hello \r\n\t")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(TrimFunctionTest, SingleSpace_ReturnsEmpty) {
    auto result = callTrim({Value(" ")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(TrimFunctionTest, TabsAndNewlines_TrimsCorrectly) {
    auto result = callTrim({Value("\t\ttest\n\n")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("test", result.asText());
}

TEST_F(TrimFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callTrim({Value(42.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("42", result.asText());
}

TEST_F(TrimFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callTrim({Value(true)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("TRUE", result.asText());
}

TEST_F(TrimFunctionTest, ErrorInput_PropagatesError) {
    auto result = callTrim({Value::error(ErrorType::NAME_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NAME_ERROR, result.asError());
}

TEST_F(TrimFunctionTest, UnicodeSpaces_HandlesBasicSpaces) {
    // Note: This test focuses on standard ASCII spaces
    auto result = callTrim({Value("   unicode test   ")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("unicode test", result.asText());
}