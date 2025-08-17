#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class SearchFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callSearch(const std::vector<Value>& args) {
        return builtin::search(args, context);
    }
};

TEST_F(SearchFunctionTest, NoArguments_ReturnsError) {
    auto result = callSearch({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SearchFunctionTest, SingleArgument_ReturnsError) {
    auto result = callSearch({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SearchFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callSearch({Value("hello"), Value("world"), Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SearchFunctionTest, EmptyFindText_ReturnsOne) {
    auto result = callSearch({Value(""), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SearchFunctionTest, EmptyWithinText_ReturnsError) {
    auto result = callSearch({Value("hello"), Value("")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SearchFunctionTest, BasicSearch_ReturnsCorrectPosition) {
    auto result = callSearch({Value("world"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(SearchFunctionTest, CaseInsensitiveSearch_IgnoresCase) {
    auto result = callSearch({Value("World"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(SearchFunctionTest, CaseInsensitiveSearch_IgnoresCaseInFindText) {
    auto result = callSearch({Value("world"), Value("hello WORLD")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(SearchFunctionTest, CaseInsensitiveSearch_BothCasesMixed) {
    auto result = callSearch({Value("WoRlD"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(SearchFunctionTest, SearchAtBeginning_ReturnsOne) {
    auto result = callSearch({Value("hello"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SearchFunctionTest, SearchAtEnd_ReturnsCorrectPosition) {
    auto result = callSearch({Value("world"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(SearchFunctionTest, SearchWithStartPosition_RespectsStart) {
    auto result = callSearch({Value("o"), Value("hello world"), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(8.0, result.asNumber());
}

TEST_F(SearchFunctionTest, StartPositionBeyondText_ReturnsError) {
    auto result = callSearch({Value("hello"), Value("world"), Value(10.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SearchFunctionTest, StartPositionLessThanOne_ReturnsError) {
    auto result = callSearch({Value("hello"), Value("world"), Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SearchFunctionTest, NonNumericStartPosition_ReturnsError) {
    auto result = callSearch({Value("hello"), Value("world"), Value("abc")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SearchFunctionTest, SearchSpecialCharacters_HandlesSpecialChars) {
    auto result = callSearch({Value("!@#"), Value("hello!@#world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(SearchFunctionTest, SearchNumbers_HandlesNumbers) {
    auto result = callSearch({Value("123"), Value("hello123world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(SearchFunctionTest, SearchWithSpaces_HandlesSpaces) {
    auto result = callSearch({Value(" "), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(SearchFunctionTest, SearchMultipleOccurrences_ReturnsFirst) {
    auto result = callSearch({Value("o"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(SearchFunctionTest, SearchNotPresent_ReturnsError) {
    auto result = callSearch({Value("xyz"), Value("hello world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SearchFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callSearch({Value(123.0), Value("hello123world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(SearchFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callSearch({Value(true), Value("helloTRUEworld")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(SearchFunctionTest, CaseInsensitiveSearch_AllUppercase) {
    auto result = callSearch({Value("WORLD"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(SearchFunctionTest, CaseInsensitiveSearch_AllLowercase) {
    auto result = callSearch({Value("world"), Value("HELLO WORLD")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(SearchFunctionTest, ErrorInput_PropagatesError) {
    auto result = callSearch({Value::error(ErrorType::DIV_ZERO), Value("hello world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(SearchFunctionTest, ErrorInSecondArgument_PropagatesError) {
    auto result = callSearch({Value("hello"), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(SearchFunctionTest, ErrorInThirdArgument_PropagatesError) {
    auto result = callSearch({Value("hello"), Value("world"), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}