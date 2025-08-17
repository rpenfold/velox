#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class CodeFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callCode(const std::vector<Value>& args) {
        return builtin::code_function(args, context);
    }
};

TEST_F(CodeFunctionTest, NoArguments_ReturnsError) {
    auto result = callCode({});
    EXPECT_TRUE(result.isError());
}

TEST_F(CodeFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callCode({Value("A"), Value("B")});
    EXPECT_TRUE(result.isError());
}

TEST_F(CodeFunctionTest, UppercaseA_Returns65) {
    auto result = callCode({Value("A")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(65.0, result.asNumber());
}

TEST_F(CodeFunctionTest, LowercaseA_Returns97) {
    auto result = callCode({Value("a")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(97.0, result.asNumber());
}

TEST_F(CodeFunctionTest, NumberCharacter_ReturnsCorrectCode) {
    auto result = callCode({Value("0")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(48.0, result.asNumber());
}

TEST_F(CodeFunctionTest, SpaceCharacter_Returns32) {
    auto result = callCode({Value(" ")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(32.0, result.asNumber());
}

TEST_F(CodeFunctionTest, MultipleCharacters_ReturnsFirstCharacterCode) {
    auto result = callCode({Value("ABC")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(65.0, result.asNumber());  // Should return code for 'A'
}

TEST_F(CodeFunctionTest, EmptyString_ReturnsError) {
    auto result = callCode({Value("")});
    EXPECT_TRUE(result.isError());
}

TEST_F(CodeFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callCode({Value(5.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(53.0, result.asNumber());  // Code for '5'
}

TEST_F(CodeFunctionTest, BooleanTrue_ConvertsToText) {
    auto result = callCode({Value(true)});
    ASSERT_TRUE(result.isNumber());
    // Boolean true converts to "TRUE", so should return code for 'T'
    EXPECT_EQ(84.0, result.asNumber());
}

TEST_F(CodeFunctionTest, BooleanFalse_ConvertsToText) {
    auto result = callCode({Value(false)});
    ASSERT_TRUE(result.isNumber());
    // Boolean false converts to "FALSE", so should return code for 'F'
    EXPECT_EQ(70.0, result.asNumber());
}

TEST_F(CodeFunctionTest, ErrorInput_PropagatesError) {
    auto result = callCode({Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
}

TEST_F(CodeFunctionTest, SpecialCharacters_ReturnsCorrectCodes) {
    auto result = callCode({Value("!")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(33.0, result.asNumber());

    result = callCode({Value("@")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(64.0, result.asNumber());
}

TEST_F(CodeFunctionTest, ExtendedASCII_ReturnsCorrectCode) {
    // Test with a character that has ASCII code > 127
    auto result = callCode({Value("€")});  // Euro symbol
    ASSERT_TRUE(result.isNumber());
    // The exact value depends on encoding, but should be > 127
    EXPECT_GT(result.asNumber(), 127.0);
}

TEST_F(CodeFunctionTest, TabCharacter_ReturnsCorrectCode) {
    auto result = callCode({Value("\t")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(9.0, result.asNumber());
}

TEST_F(CodeFunctionTest, NewlineCharacter_ReturnsCorrectCode) {
    auto result = callCode({Value("\n")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(10.0, result.asNumber());
}