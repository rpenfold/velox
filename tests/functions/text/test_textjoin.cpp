#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class TextjoinFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callTextjoin(const std::vector<Value>& args) {
        return builtin::textjoin(args, context);
    }
};

TEST_F(TextjoinFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callTextjoin({});
    EXPECT_TRUE(result.isError());

    result = callTextjoin({Value(",")});
    EXPECT_TRUE(result.isError());

    result = callTextjoin({Value(","), Value(true)});
    EXPECT_TRUE(result.isError());
}

TEST_F(TextjoinFunctionTest, BasicJoin_ReturnsJoinedText) {
    auto result = callTextjoin({Value(","), Value(true), Value("A"), Value("B"), Value("C")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A,B,C", result.asText());
}

TEST_F(TextjoinFunctionTest, JoinWithSpace_ReturnsSpaceSeparated) {
    auto result = callTextjoin({Value(" "), Value(true), Value("Hello"), Value("World")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Hello World", result.asText());
}

TEST_F(TextjoinFunctionTest, IgnoreEmptyTrue_SkipsEmptyValues) {
    auto result = callTextjoin(
            {Value("|"), Value(true), Value("A"), Value(""), Value("B"), Value(""), Value("C")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A|B|C", result.asText());
}

TEST_F(TextjoinFunctionTest, IgnoreEmptyFalse_IncludesEmptyValues) {
    auto result = callTextjoin(
            {Value("|"), Value(false), Value("A"), Value(""), Value("B"), Value(""), Value("C")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A||B||C", result.asText());
}

TEST_F(TextjoinFunctionTest, EmptyDelimiter_ConcatenatesDirectly) {
    auto result = callTextjoin({Value(""), Value(true), Value("A"), Value("B"), Value("C")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("ABC", result.asText());
}

TEST_F(TextjoinFunctionTest, SingleValue_ReturnsValueWithoutDelimiter) {
    auto result = callTextjoin({Value(","), Value(true), Value("OnlyValue")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("OnlyValue", result.asText());
}

TEST_F(TextjoinFunctionTest, AllEmptyWithIgnoreTrue_ReturnsEmpty) {
    auto result = callTextjoin({Value(","), Value(true), Value(""), Value(""), Value("")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(TextjoinFunctionTest, AllEmptyWithIgnoreFalse_ReturnsDelimiters) {
    auto result = callTextjoin({Value(","), Value(false), Value(""), Value(""), Value("")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ(",,", result.asText());
}

TEST_F(TextjoinFunctionTest, NumberInputs_ConvertsToText) {
    auto result = callTextjoin({Value("-"), Value(true), Value(1.0), Value(2.0), Value(3.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("1-2-3", result.asText());
}

TEST_F(TextjoinFunctionTest, BooleanInputs_ConvertsToText) {
    auto result = callTextjoin({Value(" "), Value(true), Value(true), Value(false), Value(true)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("TRUE FALSE TRUE", result.asText());
}

TEST_F(TextjoinFunctionTest, MixedInputTypes_ConvertsAllToText) {
    auto result = callTextjoin(
            {Value("|"), Value(true), Value("Text"), Value(123.0), Value(true), Value("End")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Text|123|TRUE|End", result.asText());
}

TEST_F(TextjoinFunctionTest, IgnoreEmptyNumericZero_TreatsZeroAsNumber) {
    // Zero is not considered empty - it's the number 0
    auto result = callTextjoin({Value(","), Value(true), Value(1.0), Value(0.0), Value(2.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("1,0,2", result.asText());
}

TEST_F(TextjoinFunctionTest, IgnoreEmptyAsNumber_ConvertsToBoolean) {
    // Non-zero number should be treated as TRUE for ignore_empty
    auto result = callTextjoin({Value(","), Value(1.0), Value("A"), Value(""), Value("B")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A,B", result.asText());

    // Zero should be treated as FALSE for ignore_empty
    result = callTextjoin({Value(","), Value(0.0), Value("A"), Value(""), Value("B")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A,,B", result.asText());
}

TEST_F(TextjoinFunctionTest, ErrorInput_PropagatesError) {
    auto result = callTextjoin({Value::error(ErrorType::VALUE_ERROR), Value(true), Value("A")});
    EXPECT_TRUE(result.isError());

    result = callTextjoin({Value(","), Value::error(ErrorType::VALUE_ERROR), Value("A")});
    EXPECT_TRUE(result.isError());

    result = callTextjoin({Value(","), Value(true), Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
}

TEST_F(TextjoinFunctionTest, LongDelimiter_WorksCorrectly) {
    auto result = callTextjoin(
            {Value(" AND "), Value(true), Value("Apple"), Value("Orange"), Value("Banana")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Apple AND Orange AND Banana", result.asText());
}

TEST_F(TextjoinFunctionTest, SpecialCharacterDelimiter_WorksCorrectly) {
    auto result = callTextjoin({Value("\t"), Value(true), Value("A"), Value("B"), Value("C")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A\tB\tC", result.asText());
}
