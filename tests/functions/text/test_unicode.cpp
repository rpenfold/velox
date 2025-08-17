#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class UnicodeFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callUnicode(const std::vector<Value>& args) {
        return builtin::unicode_function(args, context);
    }
};

TEST_F(UnicodeFunctionTest, NoArguments_ReturnsError) {
    auto result = callUnicode({});
    EXPECT_TRUE(result.isError());
}

TEST_F(UnicodeFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callUnicode({Value("A"), Value("B")});
    EXPECT_TRUE(result.isError());
}

TEST_F(UnicodeFunctionTest, BasicASCII_ReturnsCorrectCode) {
    auto result = callUnicode({Value("A")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(65.0, result.asNumber());
}

TEST_F(UnicodeFunctionTest, LowercaseASCII_ReturnsCorrectCode) {
    auto result = callUnicode({Value("a")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(97.0, result.asNumber());
}

TEST_F(UnicodeFunctionTest, SpaceCharacter_ReturnsCorrectCode) {
    auto result = callUnicode({Value(" ")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(32.0, result.asNumber());
}

TEST_F(UnicodeFunctionTest, EmptyString_ReturnsError) {
    auto result = callUnicode({Value("")});
    EXPECT_TRUE(result.isError());
}

TEST_F(UnicodeFunctionTest, MultipleCharacters_ReturnsFirstCharacterCode) {
    auto result = callUnicode({Value("ABC")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(65.0, result.asNumber());  // Should return code for 'A'
}

TEST_F(UnicodeFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callUnicode({Value(5.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(53.0, result.asNumber());  // Code for '5'
}

TEST_F(UnicodeFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callUnicode({Value(true)});
    ASSERT_TRUE(result.isNumber());
    // Boolean true converts to "TRUE", so should return code for 'T'
    EXPECT_EQ(84.0, result.asNumber());

    result = callUnicode({Value(false)});
    ASSERT_TRUE(result.isNumber());
    // Boolean false converts to "FALSE", so should return code for 'F'
    EXPECT_EQ(70.0, result.asNumber());
}

TEST_F(UnicodeFunctionTest, ErrorInput_PropagatesError) {
    auto result = callUnicode({Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
}

TEST_F(UnicodeFunctionTest, SpecialCharacters_ReturnsCorrectCodes) {
    // Newline
    auto result = callUnicode({Value("\n")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(10.0, result.asNumber());

    // Tab
    result = callUnicode({Value("\t")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(9.0, result.asNumber());

    // Exclamation mark
    result = callUnicode({Value("!")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(33.0, result.asNumber());
}

TEST_F(UnicodeFunctionTest, ExtendedASCII_ReturnsCorrectCode) {
    // Test with a character that has ASCII code > 127
    auto result = callUnicode({Value("È")});  // Latin capital E with grave
    ASSERT_TRUE(result.isNumber());
    EXPECT_GT(result.asNumber(), 127.0);
}

TEST_F(UnicodeFunctionTest, TwoByteUTF8_ReturnsCorrectCode) {
    // Latin-1 Supplement range
    auto result = callUnicode({Value("È")});  // U+00C8
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(200.0, result.asNumber());
}

TEST_F(UnicodeFunctionTest, ThreeByteUTF8_ReturnsCorrectCode) {
    // Chinese character
    auto result = callUnicode({Value("中")});  // U+4E2D
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(20013.0, result.asNumber());
}

TEST_F(UnicodeFunctionTest, FourByteUTF8_ReturnsCorrectCode) {
    // Emoji
    auto result = callUnicode({Value("😀")});  // U+1F600
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(128512.0, result.asNumber());
}

TEST_F(UnicodeFunctionTest, MixedUTF8String_ReturnsFirstCharacterCode) {
    // String with mixed character encodings - should return first char code
    auto result = callUnicode({Value("A中😀")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(65.0, result.asNumber());  // Code for 'A'
}

TEST_F(UnicodeFunctionTest, ControlCharacters_ReturnCorrectCodes) {
    // Null character (though this might be handled differently in practice)
    auto result = callUnicode({Value(std::string(1, '\x01'))});  // SOH
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(1.0, result.asNumber());
}

TEST_F(UnicodeFunctionTest, HighUnicodeValues_ReturnCorrectly) {
    // Test near the upper limit of Unicode
    auto result = callUnicode({Value("𝕏")});  // Mathematical double-struck X
    ASSERT_TRUE(result.isNumber());
    EXPECT_GT(result.asNumber(), 65536.0);  // Should be in the high Unicode range
}
