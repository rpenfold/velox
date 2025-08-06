#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class FindFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callFind(const std::vector<Value>& args) {
        return builtin::find(args, context);
    }
};

TEST_F(FindFunctionTest, NoArguments_ReturnsError) {
    auto result = callFind({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(FindFunctionTest, SingleArgument_ReturnsError) {
    auto result = callFind({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(FindFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callFind({Value("hello"), Value("world"), Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(FindFunctionTest, EmptyFindText_ReturnsOne) {
    auto result = callFind({Value(""), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(FindFunctionTest, EmptyWithinText_ReturnsError) {
    auto result = callFind({Value("hello"), Value("")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(FindFunctionTest, BasicFind_ReturnsCorrectPosition) {
    auto result = callFind({Value("world"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(FindFunctionTest, CaseSensitiveFind_RespectsCase) {
    auto result = callFind({Value("World"), Value("hello world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(FindFunctionTest, FindAtBeginning_ReturnsOne) {
    auto result = callFind({Value("hello"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(FindFunctionTest, FindAtEnd_ReturnsCorrectPosition) {
    auto result = callFind({Value("world"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(FindFunctionTest, FindWithStartPosition_RespectsStart) {
    auto result = callFind({Value("o"), Value("hello world"), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(8.0, result.asNumber());
}

TEST_F(FindFunctionTest, StartPositionBeyondText_ReturnsError) {
    auto result = callFind({Value("hello"), Value("world"), Value(10.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(FindFunctionTest, StartPositionLessThanOne_ReturnsError) {
    auto result = callFind({Value("hello"), Value("world"), Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(FindFunctionTest, NonNumericStartPosition_ReturnsError) {
    auto result = callFind({Value("hello"), Value("world"), Value("abc")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(FindFunctionTest, FindSpecialCharacters_HandlesSpecialChars) {
    auto result = callFind({Value("!@#"), Value("hello!@#world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(FindFunctionTest, FindNumbers_HandlesNumbers) {
    auto result = callFind({Value("123"), Value("hello123world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(FindFunctionTest, FindWithSpaces_HandlesSpaces) {
    auto result = callFind({Value(" "), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(FindFunctionTest, FindMultipleOccurrences_ReturnsFirst) {
    auto result = callFind({Value("o"), Value("hello world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(FindFunctionTest, FindNotPresent_ReturnsError) {
    auto result = callFind({Value("xyz"), Value("hello world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(FindFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callFind({Value(123.0), Value("hello123world")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(FindFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callFind({Value(true), Value("helloTRUEworld")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(FindFunctionTest, ErrorInput_PropagatesError) {
    auto result = callFind({Value::error(ErrorType::DIV_ZERO), Value("hello world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(FindFunctionTest, ErrorInSecondArgument_PropagatesError) {
    auto result = callFind({Value("hello"), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(FindFunctionTest, ErrorInThirdArgument_PropagatesError) {
    auto result = callFind({Value("hello"), Value("world"), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}