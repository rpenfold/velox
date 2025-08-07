#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class TFunctionTest : public ::testing::Test {
protected:
    Context context;
    
    Value callT(const std::vector<Value>& args) {
        return builtin::t_function(args, context);
    }
};

TEST_F(TFunctionTest, NoArguments_ReturnsError) {
    auto result = callT({});
    EXPECT_TRUE(result.isError());
}

TEST_F(TFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callT({Value("text"), Value("extra")});
    EXPECT_TRUE(result.isError());
}

TEST_F(TFunctionTest, TextInput_ReturnsTextUnchanged) {
    auto result = callT({Value("Hello World")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Hello World", result.asText());
}

TEST_F(TFunctionTest, EmptyTextInput_ReturnsEmptyText) {
    auto result = callT({Value("")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(TFunctionTest, NumberInput_ReturnsEmptyString) {
    auto result = callT({Value(123.45)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(TFunctionTest, BooleanTrueInput_ReturnsEmptyString) {
    auto result = callT({Value(true)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(TFunctionTest, BooleanFalseInput_ReturnsEmptyString) {
    auto result = callT({Value(false)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(TFunctionTest, ErrorInput_PropagatesError) {
    auto result = callT({Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
}

TEST_F(TFunctionTest, ZeroInput_ReturnsEmptyString) {
    auto result = callT({Value(0.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(TFunctionTest, SpecialTextCharacters_ReturnsTextUnchanged) {
    auto result = callT({Value("!@#$%^&*()")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("!@#$%^&*()", result.asText());
}

TEST_F(TFunctionTest, UnicodeText_ReturnsTextUnchanged) {
    auto result = callT({Value("Hello 世界")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Hello 世界", result.asText());
}

TEST_F(TFunctionTest, NumericTextString_ReturnsTextUnchanged) {
    // Text that looks like a number should still return the text
    auto result = callT({Value("123.45")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("123.45", result.asText());
}
