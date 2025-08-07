#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/context.h"

class RptFunctionTest : public ::testing::Test {
protected:
    Context context;
};

TEST_F(RptFunctionTest, ValidInput_ReturnsCorrectResult) {
    // Arrange
    std::vector<Value> args = {Value("abc"), Value(3.0)};
    
    // Act
    Value result = builtin::rpt(args, context);
    
    // Assert
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("abcabcabc", result.asText());
}

TEST_F(RptFunctionTest, SingleRepeat_ReturnsOriginalText) {
    // Arrange
    std::vector<Value> args = {Value("hello"), Value(1.0)};
    
    // Act
    Value result = builtin::rpt(args, context);
    
    // Assert
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(RptFunctionTest, ZeroRepeat_ReturnsEmptyString) {
    // Arrange
    std::vector<Value> args = {Value("hello"), Value(0.0)};
    
    // Act
    Value result = builtin::rpt(args, context);
    
    // Assert
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(RptFunctionTest, DecimalRepeat_TruncatesToInteger) {
    // Arrange
    std::vector<Value> args = {Value("abc"), Value(2.7)};
    
    // Act
    Value result = builtin::rpt(args, context);
    
    // Assert
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("abcabc", result.asText());
}

TEST_F(RptFunctionTest, EmptyString_RepeatsCorrectly) {
    // Arrange
    std::vector<Value> args = {Value(""), Value(3.0)};
    
    // Act
    Value result = builtin::rpt(args, context);
    
    // Assert
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(RptFunctionTest, LargeRepeat_HandlesCorrectly) {
    // Arrange
    std::vector<Value> args = {Value("x"), Value(10.0)};
    
    // Act
    Value result = builtin::rpt(args, context);
    
    // Assert
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("xxxxxxxxxx", result.asText());
}

TEST_F(RptFunctionTest, InvalidArgumentCount_ThrowsException) {
    // Arrange
    std::vector<Value> args = {Value("hello")};
    
    // Act & Assert
    EXPECT_THROW(builtin::rpt(args, context), std::runtime_error);
}

TEST_F(RptFunctionTest, TooManyArguments_ThrowsException) {
    // Arrange
    std::vector<Value> args = {Value("hello"), Value(2.0), Value(3.0)};
    
    // Act & Assert
    EXPECT_THROW(builtin::rpt(args, context), std::runtime_error);
}

TEST_F(RptFunctionTest, NonTextFirstArgument_ThrowsException) {
    // Arrange
    std::vector<Value> args = {Value(123.0), Value(2.0)};
    
    // Act & Assert
    EXPECT_THROW(builtin::rpt(args, context), std::runtime_error);
}

TEST_F(RptFunctionTest, NonNumericSecondArgument_ThrowsException) {
    // Arrange
    std::vector<Value> args = {Value("hello"), Value("world")};
    
    // Act & Assert
    EXPECT_THROW(builtin::rpt(args, context), std::runtime_error);
}

TEST_F(RptFunctionTest, NegativeRepeatCount_ThrowsException) {
    // Arrange
    std::vector<Value> args = {Value("hello"), Value(-1.0)};
    
    // Act & Assert
    EXPECT_THROW(builtin::rpt(args, context), std::runtime_error);
}

TEST_F(RptFunctionTest, SpecialCharacters_RepeatsCorrectly) {
    // Arrange
    std::vector<Value> args = {Value("!@#$%"), Value(2.0)};
    
    // Act
    Value result = builtin::rpt(args, context);
    
    // Assert
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("!@#$%!@#$%", result.asText());
}

TEST_F(RptFunctionTest, UnicodeCharacters_RepeatsCorrectly) {
    // Arrange
    std::vector<Value> args = {Value("🚀🌟"), Value(2.0)};
    
    // Act
    Value result = builtin::rpt(args, context);
    
    // Assert
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("🚀🌟🚀🌟", result.asText());
} 