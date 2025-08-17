#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class MidFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callMid(const std::vector<Value>& args) {
        return builtin::mid(args, context);
    }
};

TEST_F(MidFunctionTest, NoArguments_ReturnsError) {
    auto result = callMid({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MidFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callMid({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MidFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callMid({Value("hello"), Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MidFunctionTest, ValidArguments_ReturnsCorrectSubstring) {
    auto result = callMid({Value("hello"), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("ell", result.asText());
}

TEST_F(MidFunctionTest, StartNumOne_ReturnsFromBeginning) {
    auto result = callMid({Value("hello"), Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("he", result.asText());
}

TEST_F(MidFunctionTest, StartNumBeyondLength_ReturnsEmptyString) {
    auto result = callMid({Value("hello"), Value(10.0), Value(2.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(MidFunctionTest, NumCharsBeyondEnd_ReturnsAvailableCharacters) {
    auto result = callMid({Value("hello"), Value(4.0), Value(10.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("lo", result.asText());
}

TEST_F(MidFunctionTest, StartNumZero_ReturnsError) {
    auto result = callMid({Value("hello"), Value(0.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MidFunctionTest, StartNumNegative_ReturnsError) {
    auto result = callMid({Value("hello"), Value(-1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MidFunctionTest, NumCharsNegative_ReturnsEmptyString) {
    auto result = callMid({Value("hello"), Value(1.0), Value(-1.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(MidFunctionTest, NumCharsZero_ReturnsEmptyString) {
    auto result = callMid({Value("hello"), Value(1.0), Value(0.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(MidFunctionTest, NonNumericStartNum_ReturnsError) {
    auto result = callMid({Value("hello"), Value("abc"), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MidFunctionTest, NonNumericNumChars_ReturnsError) {
    auto result = callMid({Value("hello"), Value(1.0), Value("abc")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MidFunctionTest, EmptyString_ReturnsEmptyString) {
    auto result = callMid({Value(""), Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(MidFunctionTest, SingleCharacter_ReturnsCharacter) {
    auto result = callMid({Value("a"), Value(1.0), Value(1.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("a", result.asText());
}

TEST_F(MidFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callMid({Value(123.45), Value(2.0), Value(2.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("23", result.asText());
}

TEST_F(MidFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callMid({Value(true), Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("TR", result.asText());
}

TEST_F(MidFunctionTest, TextWithSpaces_HandlesSpaces) {
    auto result = callMid({Value("hello world"), Value(7.0), Value(5.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("world", result.asText());
}

TEST_F(MidFunctionTest, TextWithSpecialCharacters_HandlesSpecialChars) {
    auto result = callMid({Value("hello!@#"), Value(6.0), Value(3.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("!@#", result.asText());
}

TEST_F(MidFunctionTest, TextWithNumbers_HandlesNumbers) {
    auto result = callMid({Value("abc123"), Value(4.0), Value(3.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("123", result.asText());
}

TEST_F(MidFunctionTest, TextWithNewlines_HandlesNewlines) {
    auto result = callMid({Value("line1\nline2"), Value(6.0), Value(5.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("\nline", result.asText());
}

TEST_F(MidFunctionTest, ErrorInput_PropagatesError) {
    auto result = callMid({Value::error(ErrorType::DIV_ZERO), Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(MidFunctionTest, ErrorInSecondArgument_PropagatesError) {
    auto result = callMid({Value("hello"), Value::error(ErrorType::DIV_ZERO), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(MidFunctionTest, ErrorInThirdArgument_PropagatesError) {
    auto result = callMid({Value("hello"), Value(1.0), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}