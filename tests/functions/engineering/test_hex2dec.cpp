#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class Hex2DecFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callHex2Dec(const std::vector<Value>& args) {
        return hex2dec(args, context);
    }
};

// Basic functionality tests
TEST_F(Hex2DecFunctionTest, NoArguments_ReturnsError) {
    auto result = callHex2Dec({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(Hex2DecFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callHex2Dec({Value("A"), Value("B")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Valid conversions
TEST_F(Hex2DecFunctionTest, Zero_ReturnsZero) {
    auto result = callHex2Dec({Value("0")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(0.0, result.asNumber());
}

TEST_F(Hex2DecFunctionTest, SingleDigit_ConvertsCorrectly) {
    auto result = callHex2Dec({Value("A")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(10.0, result.asNumber());
}

TEST_F(Hex2DecFunctionTest, MultipleDigits_ConvertsCorrectly) {
    auto result = callHex2Dec({Value("FF")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(255.0, result.asNumber());
}

TEST_F(Hex2DecFunctionTest, LargeHex_ConvertsCorrectly) {
    auto result = callHex2Dec({Value("1000")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(4096.0, result.asNumber());
}

TEST_F(Hex2DecFunctionTest, MaxValue_ConvertsCorrectly) {
    auto result = callHex2Dec({Value("7FFFFFFFFF")});  // Max positive 40-bit value
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(549755813887.0, result.asNumber());  // 2^39 - 1
}

// Case insensitive
TEST_F(Hex2DecFunctionTest, LowerCase_ConvertsCorrectly) {
    auto result = callHex2Dec({Value("abc")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(2748.0, result.asNumber());
}

TEST_F(Hex2DecFunctionTest, MixedCase_ConvertsCorrectly) {
    auto result = callHex2Dec({Value("AbC")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(2748.0, result.asNumber());
}

// Negative numbers (two's complement)
TEST_F(Hex2DecFunctionTest, NegativeNumber_ConvertsCorrectly) {
    auto result = callHex2Dec({Value("FFFFFFFFFE")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(-2.0, result.asNumber());
}

TEST_F(Hex2DecFunctionTest, NegativeOne_ConvertsCorrectly) {
    auto result = callHex2Dec({Value("FFFFFFFFFF")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(-1.0, result.asNumber());
}

// Error cases
TEST_F(Hex2DecFunctionTest, InvalidCharacter_ReturnsError) {
    auto result = callHex2Dec({Value("G")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Hex2DecFunctionTest, InvalidCharacterInMiddle_ReturnsError) {
    auto result = callHex2Dec({Value("A1G")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Hex2DecFunctionTest, TooLong_ReturnsError) {
    auto result = callHex2Dec({Value("12345678901")});  // 11 digits
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Hex2DecFunctionTest, EmptyString_ReturnsZero) {
    auto result = callHex2Dec({Value("")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(0.0, result.asNumber());
}

// Whitespace handling
TEST_F(Hex2DecFunctionTest, WithSpaces_HandlesCorrectly) {
    auto result = callHex2Dec({Value(" A B ")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(171.0, result.asNumber());  // "AB" = 171
}

// Number input conversion
TEST_F(Hex2DecFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callHex2Dec({Value(10.0)});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(16.0, result.asNumber());  // "10" in hex = 16 in decimal
}

// Error propagation
TEST_F(Hex2DecFunctionTest, ErrorInput_PropagatesError) {
    auto result = callHex2Dec({Value::error(ErrorType::DIV_ZERO)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

// Boolean input
TEST_F(Hex2DecFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callHex2Dec({Value(true)});
    EXPECT_TRUE(result.isError());  // "TRUE" contains invalid hex characters
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}