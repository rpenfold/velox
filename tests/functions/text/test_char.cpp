#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class CharFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callChar(const std::vector<Value>& args) {
        return builtin::char_function(args, context);
    }
};

TEST_F(CharFunctionTest, NoArguments_ReturnsError) {
    auto result = callChar({});
    EXPECT_TRUE(result.isError());
}

TEST_F(CharFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callChar({Value(65.0), Value(66.0)});
    EXPECT_TRUE(result.isError());
}

TEST_F(CharFunctionTest, BasicASCII_ReturnsCorrectCharacter) {
    auto result = callChar({Value(65.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(CharFunctionTest, LowercaseASCII_ReturnsCorrectCharacter) {
    auto result = callChar({Value(97.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("a", result.asText());
}

TEST_F(CharFunctionTest, NumberCharacter_ReturnsCorrectCharacter) {
    auto result = callChar({Value(48.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());
}

TEST_F(CharFunctionTest, SpaceCharacter_ReturnsSpace) {
    auto result = callChar({Value(32.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ(" ", result.asText());
}

TEST_F(CharFunctionTest, InvalidRange_TooLow_ReturnsError) {
    auto result = callChar({Value(0.0)});
    EXPECT_TRUE(result.isError());
}

TEST_F(CharFunctionTest, InvalidRange_TooHigh_ReturnsError) {
    auto result = callChar({Value(256.0)});
    EXPECT_TRUE(result.isError());
}

TEST_F(CharFunctionTest, FloatingPoint_TruncatesInteger) {
    auto result = callChar({Value(65.7)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(CharFunctionTest, TextInput_ConvertsToNumber) {
    auto result = callChar({Value("65")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(CharFunctionTest, BooleanTrue_ConvertsToOne) {
    auto result = callChar({Value(true)});
    ASSERT_TRUE(result.isText());            // 1 is valid ASCII range for CHAR
    EXPECT_EQ(1, result.asText().length());  // Should return one character
}

TEST_F(CharFunctionTest, BooleanFalse_ConvertsToZero) {
    auto result = callChar({Value(false)});
    EXPECT_TRUE(result.isError());  // 0 is not in valid ASCII range for CHAR
}

TEST_F(CharFunctionTest, ErrorInput_PropagatesError) {
    auto result = callChar({Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
}

TEST_F(CharFunctionTest, NonNumericText_ReturnsError) {
    auto result = callChar({Value("hello")});
    EXPECT_TRUE(result.isError());
}

TEST_F(CharFunctionTest, NegativeNumber_ReturnsError) {
    auto result = callChar({Value(-1.0)});
    EXPECT_TRUE(result.isError());
}

TEST_F(CharFunctionTest, ExtendedASCII_ReturnsCorrectCharacter) {
    auto result = callChar({Value(128.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ(1, result.asText().length());  // Should return a character
}