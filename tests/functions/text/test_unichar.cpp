#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class UnicharFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callUnichar(const std::vector<Value>& args) {
        return builtin::unichar(args, context);
    }
};

TEST_F(UnicharFunctionTest, NoArguments_ReturnsError) {
    auto result = callUnichar({});
    EXPECT_TRUE(result.isError());
}

TEST_F(UnicharFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callUnichar({Value(65.0), Value(66.0)});
    EXPECT_TRUE(result.isError());
}

TEST_F(UnicharFunctionTest, BasicASCII_ReturnsCorrectCharacter) {
    auto result = callUnichar({Value(65.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(UnicharFunctionTest, LowercaseASCII_ReturnsCorrectCharacter) {
    auto result = callUnichar({Value(97.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("a", result.asText());
}

TEST_F(UnicharFunctionTest, SpaceCharacter_ReturnsSpace) {
    auto result = callUnichar({Value(32.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ(" ", result.asText());
}

TEST_F(UnicharFunctionTest, InvalidRange_TooLow_ReturnsError) {
    auto result = callUnichar({Value(0.0)});
    EXPECT_TRUE(result.isError());

    result = callUnichar({Value(-1.0)});
    EXPECT_TRUE(result.isError());
}

TEST_F(UnicharFunctionTest, InvalidRange_TooHigh_ReturnsError) {
    auto result = callUnichar({Value(1114112.0)});  // > 0x10FFFF
    EXPECT_TRUE(result.isError());
}

TEST_F(UnicharFunctionTest, MaxValidCodePoint_ReturnsCharacter) {
    auto result = callUnichar({Value(1114111.0)});  // 0x10FFFF
    ASSERT_TRUE(result.isText());
    EXPECT_GE(result.asText().length(), 1);
}

TEST_F(UnicharFunctionTest, FloatingPoint_TruncatesInteger) {
    auto result = callUnichar({Value(65.7)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(UnicharFunctionTest, TextInput_ConvertsToNumber) {
    auto result = callUnichar({Value("65")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(UnicharFunctionTest, NonNumericText_ReturnsError) {
    auto result = callUnichar({Value("hello")});
    EXPECT_TRUE(result.isError());
}

TEST_F(UnicharFunctionTest, ErrorInput_PropagatesError) {
    auto result = callUnichar({Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
}

TEST_F(UnicharFunctionTest, ExtendedASCII_ReturnsCorrectCharacter) {
    auto result = callUnichar({Value(128.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_GE(result.asText().length(),
              1);  // Should return a character (may be multi-byte in UTF-8)
}

TEST_F(UnicharFunctionTest, TwoByteUTF8_ReturnsCorrectCharacter) {
    // Latin-1 Supplement range (2-byte UTF-8)
    auto result = callUnichar({Value(200.0)});  // È
    ASSERT_TRUE(result.isText());
    EXPECT_GE(result.asText().length(), 1);
}

TEST_F(UnicharFunctionTest, ThreeByteUTF8_ReturnsCorrectCharacter) {
    // Chinese character (3-byte UTF-8)
    auto result = callUnichar({Value(20013.0)});  // 中
    ASSERT_TRUE(result.isText());
    EXPECT_GE(result.asText().length(), 1);
}

TEST_F(UnicharFunctionTest, FourByteUTF8_ReturnsCorrectCharacter) {
    // Emoji (4-byte UTF-8)
    auto result = callUnichar({Value(128512.0)});  // 😀
    ASSERT_TRUE(result.isText());
    EXPECT_GE(result.asText().length(), 1);
}

TEST_F(UnicharFunctionTest, SpecialCharacters_ReturnCorrectly) {
    // Newline
    auto result = callUnichar({Value(10.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("\n", result.asText());

    // Tab
    result = callUnichar({Value(9.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("\t", result.asText());
}

TEST_F(UnicharFunctionTest, BooleanInput_ConvertsToNumber) {
    auto result = callUnichar({Value(true)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ(1, result.asText().length());  // Character for code 1

    result = callUnichar({Value(false)});
    EXPECT_TRUE(result.isError());  // 0 is invalid
}
