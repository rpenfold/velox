#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class ExactFunctionTest : public ::testing::Test {
protected:
    Context context;
    
    Value callExact(const std::vector<Value>& args) {
        return builtin::exact(args, context);
    }
};

TEST_F(ExactFunctionTest, NoArguments_ReturnsError) {
    auto result = callExact({});
    EXPECT_TRUE(result.isError());
}

TEST_F(ExactFunctionTest, OneArgument_ReturnsError) {
    auto result = callExact({Value("text")});
    EXPECT_TRUE(result.isError());
}

TEST_F(ExactFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callExact({Value("text1"), Value("text2"), Value("text3")});
    EXPECT_TRUE(result.isError());
}

TEST_F(ExactFunctionTest, IdenticalStrings_ReturnsTrue) {
    auto result = callExact({Value("Hello"), Value("Hello")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(ExactFunctionTest, DifferentCase_ReturnsFalse) {
    auto result = callExact({Value("Hello"), Value("hello")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
    
    result = callExact({Value("HELLO"), Value("hello")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(ExactFunctionTest, DifferentStrings_ReturnsFalse) {
    auto result = callExact({Value("Hello"), Value("World")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(ExactFunctionTest, EmptyStrings_ReturnsTrue) {
    auto result = callExact({Value(""), Value("")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(ExactFunctionTest, EmptyVsNonEmpty_ReturnsFalse) {
    auto result = callExact({Value(""), Value("text")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
    
    result = callExact({Value("text"), Value("")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(ExactFunctionTest, WhitespaceMatters) {
    auto result = callExact({Value("Hello"), Value("Hello ")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
    
    result = callExact({Value(" Hello"), Value("Hello")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(ExactFunctionTest, NumbersAsText_ComparesCorrectly) {
    auto result = callExact({Value("123"), Value("123")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
    
    result = callExact({Value("123"), Value("124")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(ExactFunctionTest, NumberInputs_ConvertToText) {
    auto result = callExact({Value(123.0), Value(123.0)});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
    
    result = callExact({Value(123.0), Value(124.0)});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(ExactFunctionTest, NumberVsTextNumber_ComparesCorrectly) {
    auto result = callExact({Value(123.0), Value("123")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(ExactFunctionTest, BooleanInputs_ConvertToText) {
    auto result = callExact({Value(true), Value(true)});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
    
    result = callExact({Value(true), Value(false)});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
    
    result = callExact({Value(true), Value("TRUE")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
    
    result = callExact({Value(false), Value("FALSE")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(ExactFunctionTest, ErrorInput_PropagatesError) {
    auto result = callExact({Value::error(ErrorType::VALUE_ERROR), Value("text")});
    EXPECT_TRUE(result.isError());
    
    result = callExact({Value("text"), Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
}

TEST_F(ExactFunctionTest, SpecialCharacters_ComparesCorrectly) {
    auto result = callExact({Value("Hello!@#$%"), Value("Hello!@#$%")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
    
    result = callExact({Value("Hello!@#$%"), Value("Hello!@#$&")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(ExactFunctionTest, UnicodeCharacters_ComparesCorrectly) {
    auto result = callExact({Value("Héllo"), Value("Héllo")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
    
    result = callExact({Value("Héllo"), Value("Hello")});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(ExactFunctionTest, LongStrings_ComparesCorrectly) {
    std::string longString1(1000, 'A');
    std::string longString2(1000, 'A');
    std::string longString3(999, 'A');
    longString3 += 'B';
    
    auto result = callExact({Value(longString1), Value(longString2)});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
    
    result = callExact({Value(longString1), Value(longString3)});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}