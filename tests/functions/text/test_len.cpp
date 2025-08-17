#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class LenFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callLen(const std::vector<Value>& args) {
        return builtin::len(args, context);
    }
};

TEST_F(LenFunctionTest, NoArguments_ReturnsError) {
    auto result = callLen({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LenFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callLen({Value("a"), Value("b")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(LenFunctionTest, SimpleText_ReturnsCorrectLength) {
    auto result = callLen({Value("hello")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(LenFunctionTest, EmptyString_ReturnsZero) {
    auto result = callLen({Value("")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(LenFunctionTest, SingleCharacter_ReturnsOne) {
    auto result = callLen({Value("a")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(LenFunctionTest, TextWithSpaces_CountsSpaces) {
    auto result = callLen({Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(11.0, result.asNumber());
}

TEST_F(LenFunctionTest, TextWithLeadingTrailingSpaces_CountsAllSpaces) {
    auto result = callLen({Value("  hello  ")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(9.0, result.asNumber());
}

TEST_F(LenFunctionTest, TextWithSpecialCharacters_CountsAll) {
    auto result = callLen({Value("hello!@#$%")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(10.0, result.asNumber());
}

TEST_F(LenFunctionTest, TextWithNumbers_CountsAll) {
    auto result = callLen({Value("abc123")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(LenFunctionTest, TextWithNewlines_CountsNewlines) {
    auto result = callLen({Value("line1\nline2")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(11.0, result.asNumber());
}

TEST_F(LenFunctionTest, TextWithTabs_CountsTabs) {
    auto result = callLen({Value("hello\tworld")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(11.0, result.asNumber());
}

TEST_F(LenFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callLen({Value(42.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // "42"
}

TEST_F(LenFunctionTest, NegativeNumberInput_ConvertsToTextFirst) {
    auto result = callLen({Value(-123.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());  // "-123"
}

TEST_F(LenFunctionTest, DecimalNumberInput_ConvertsToTextFirst) {
    auto result = callLen({Value(3.14)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());  // "3.14"
}

TEST_F(LenFunctionTest, TrueBooleanInput_ConvertsToTextFirst) {
    auto result = callLen({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());  // "TRUE"
}

TEST_F(LenFunctionTest, FalseBooleanInput_ConvertsToTextFirst) {
    auto result = callLen({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());  // "FALSE"
}

TEST_F(LenFunctionTest, ErrorInput_PropagatesError) {
    auto result = callLen({Value::error(ErrorType::REF_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(LenFunctionTest, VeryLongString_ReturnsCorrectLength) {
    std::string longString(1000, 'a');
    auto result = callLen({Value(longString)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1000.0, result.asNumber());
}