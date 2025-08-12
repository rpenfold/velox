#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class RptFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callRpt(const std::vector<Value>& args) {
        return builtin::rpt(args, context);
    }
};

TEST_F(RptFunctionTest, NoArguments_ReturnsError) {
    auto result = callRpt({});
    EXPECT_TRUE(result.isError());
}

TEST_F(RptFunctionTest, OneArgument_ReturnsError) {
    auto result = callRpt({Value("text")});
    EXPECT_TRUE(result.isError());
}

TEST_F(RptFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callRpt({Value("text"), Value(3.0), Value("extra")});
    EXPECT_TRUE(result.isError());
}

TEST_F(RptFunctionTest, BasicRepeat_ReturnsRepeatedText) {
    auto result = callRpt({Value("Hi"), Value(3.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("HiHiHi", result.asText());
}

TEST_F(RptFunctionTest, RepeatZero_ReturnsEmptyString) {
    auto result = callRpt({Value("Hello"), Value(0.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(RptFunctionTest, RepeatOne_ReturnsOriginalText) {
    auto result = callRpt({Value("World"), Value(1.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("World", result.asText());
}

TEST_F(RptFunctionTest, EmptyString_ReturnsEmpty) {
    auto result = callRpt({Value(""), Value(5.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(RptFunctionTest, NegativeCount_ReturnsEmptyString) {
    auto result = callRpt({Value("Test"), Value(-1.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(RptFunctionTest, FloatingPointCount_TruncatesInteger) {
    auto result = callRpt({Value("AB"), Value(2.7)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("ABAB", result.asText());
}

TEST_F(RptFunctionTest, LargeCount_HandlesLargeRepetition) {
    auto result = callRpt({Value("X"), Value(10.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("XXXXXXXXXX", result.asText());
}

TEST_F(RptFunctionTest, NumberInput_ConvertsToText) {
    auto result = callRpt({Value(123.0), Value(2.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("123123", result.asText());
}

TEST_F(RptFunctionTest, BooleanInput_ConvertsToText) {
    auto result = callRpt({Value(true), Value(2.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("TRUETRUE", result.asText());

    result = callRpt({Value(false), Value(3.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("FALSEFALSEFALSE", result.asText());
}

TEST_F(RptFunctionTest, TextCount_ConvertsToNumber) {
    auto result = callRpt({Value("OK"), Value("3")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("OKOKOK", result.asText());
}

TEST_F(RptFunctionTest, NonNumericCount_ReturnsError) {
    auto result = callRpt({Value("Test"), Value("hello")});
    EXPECT_TRUE(result.isError());
}

TEST_F(RptFunctionTest, ErrorInput_PropagatesError) {
    auto result = callRpt({Value::error(ErrorType::VALUE_ERROR), Value(2.0)});
    EXPECT_TRUE(result.isError());

    result = callRpt({Value("Text"), Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
}

TEST_F(RptFunctionTest, SpecialCharacters_RepeatsCorrectly) {
    auto result = callRpt({Value("!@#"), Value(2.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("!@#!@#", result.asText());
}

TEST_F(RptFunctionTest, Whitespace_RepeatsCorrectly) {
    auto result = callRpt({Value(" "), Value(5.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("     ", result.asText());
}

TEST_F(RptFunctionTest, UnicodeCharacters_RepeatsCorrectly) {
    auto result = callRpt({Value("café"), Value(2.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("cafécafé", result.asText());
}
