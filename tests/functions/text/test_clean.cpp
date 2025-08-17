#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class CleanFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callClean(const std::vector<Value>& args) {
        return builtin::clean(args, context);
    }
};

TEST_F(CleanFunctionTest, NoArguments_ReturnsError) {
    auto result = callClean({});
    EXPECT_TRUE(result.isError());
}

TEST_F(CleanFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callClean({Value("text"), Value("extra")});
    EXPECT_TRUE(result.isError());
}

TEST_F(CleanFunctionTest, PlainText_RemainsUnchanged) {
    auto result = callClean({Value("Hello World")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Hello World", result.asText());
}

TEST_F(CleanFunctionTest, RemovesTabCharacters) {
    auto result = callClean({Value("Hello\tWorld")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("HelloWorld", result.asText());
}

TEST_F(CleanFunctionTest, RemovesNewlineCharacters) {
    auto result = callClean({Value("Hello\nWorld")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("HelloWorld", result.asText());
}

TEST_F(CleanFunctionTest, RemovesCarriageReturn) {
    auto result = callClean({Value("Hello\rWorld")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("HelloWorld", result.asText());
}

TEST_F(CleanFunctionTest, RemovesMultipleNonPrintableCharacters) {
    auto result = callClean({Value("Hello\t\n\rWorld\t\n")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("HelloWorld", result.asText());
}

TEST_F(CleanFunctionTest, EmptyString_RemainsEmpty) {
    auto result = callClean({Value("")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(CleanFunctionTest, OnlyNonPrintableCharacters_ReturnsEmpty) {
    auto result = callClean({Value("\t\n\r")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(CleanFunctionTest, PreservesSpaces) {
    auto result = callClean({Value("Hello World")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Hello World", result.asText());
}

TEST_F(CleanFunctionTest, PreservesNumbers) {
    auto result = callClean({Value("123\t456\n789")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("123456789", result.asText());
}

TEST_F(CleanFunctionTest, PreservesSpecialCharacters) {
    auto result = callClean({Value("Hello!@#$%^&*()")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Hello!@#$%^&*()", result.asText());
}

TEST_F(CleanFunctionTest, NumberInput_ConvertsToText) {
    auto result = callClean({Value(123.45)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("123.45", result.asText());
}

TEST_F(CleanFunctionTest, BooleanInput_ConvertsToText) {
    auto result = callClean({Value(true)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("TRUE", result.asText());

    result = callClean({Value(false)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("FALSE", result.asText());
}

TEST_F(CleanFunctionTest, ErrorInput_PropagatesError) {
    auto result = callClean({Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
}

TEST_F(CleanFunctionTest, RemovesControlCharacters) {
    // Test with various control characters (ASCII 0-31 except space)
    std::string input = "Hello";
    input += char(1);   // SOH
    input += char(7);   // BEL
    input += char(27);  // ESC
    input += "World";

    auto result = callClean({Value(input)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("HelloWorld", result.asText());
}

TEST_F(CleanFunctionTest, PreservesUnicodeCharacters) {
    auto result = callClean({Value("Héllo Wörld")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Héllo Wörld", result.asText());
}