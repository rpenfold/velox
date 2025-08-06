#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class Bin2DecFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callBin2Dec(const std::vector<Value>& args) {
        return bin2dec(args, context);
    }
};

// Basic functionality tests
TEST_F(Bin2DecFunctionTest, NoArguments_ReturnsError) {
    auto result = callBin2Dec({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(Bin2DecFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callBin2Dec({Value("1"), Value("0")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Valid conversions
TEST_F(Bin2DecFunctionTest, Zero_ReturnsZero) {
    auto result = callBin2Dec({Value("0")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(0.0, result.asNumber());
}

TEST_F(Bin2DecFunctionTest, One_ReturnsOne) {
    auto result = callBin2Dec({Value("1")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(1.0, result.asNumber());
}

TEST_F(Bin2DecFunctionTest, SimplePattern_ConvertsCorrectly) {
    auto result = callBin2Dec({Value("101")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(5.0, result.asNumber());
}

TEST_F(Bin2DecFunctionTest, AllOnes_ConvertsCorrectly) {
    auto result = callBin2Dec({Value("1111")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(15.0, result.asNumber());
}

TEST_F(Bin2DecFunctionTest, MaxPositive_ConvertsCorrectly) {
    auto result = callBin2Dec({Value("0111111111")});  // 511
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(511.0, result.asNumber());
}

// Negative numbers (two's complement)
TEST_F(Bin2DecFunctionTest, NegativeOne_ConvertsCorrectly) {
    auto result = callBin2Dec({Value("1111111111")});  // -1 in 10-bit two's complement
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(-1.0, result.asNumber());
}

TEST_F(Bin2DecFunctionTest, NegativeTwo_ConvertsCorrectly) {
    auto result = callBin2Dec({Value("1111111110")});  // -2 in 10-bit two's complement
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(-2.0, result.asNumber());
}

TEST_F(Bin2DecFunctionTest, MaxNegative_ConvertsCorrectly) {
    auto result = callBin2Dec({Value("1000000000")});  // -512 in 10-bit two's complement
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(-512.0, result.asNumber());
}

// Error cases
TEST_F(Bin2DecFunctionTest, InvalidCharacter_ReturnsError) {
    auto result = callBin2Dec({Value("2")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Bin2DecFunctionTest, InvalidCharacterInMiddle_ReturnsError) {
    auto result = callBin2Dec({Value("1021")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Bin2DecFunctionTest, TooLong_ReturnsError) {
    auto result = callBin2Dec({Value("10101010101")});  // 11 digits
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Bin2DecFunctionTest, EmptyString_ReturnsZero) {
    auto result = callBin2Dec({Value("")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(0.0, result.asNumber());
}

// Whitespace handling
TEST_F(Bin2DecFunctionTest, WithSpaces_HandlesCorrectly) {
    auto result = callBin2Dec({Value(" 1 0 1 ")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(5.0, result.asNumber());  // "101" = 5
}

// Type conversions
TEST_F(Bin2DecFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callBin2Dec({Value(101.0)});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(5.0, result.asNumber());  // "101" in binary = 5 in decimal
}

TEST_F(Bin2DecFunctionTest, BooleanTrue_ConvertsCorrectly) {
    auto result = callBin2Dec({Value(true)});
    EXPECT_TRUE(result.isError());  // "TRUE" contains invalid binary characters
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// Error propagation
TEST_F(Bin2DecFunctionTest, ErrorInput_PropagatesError) {
    auto result = callBin2Dec({Value::error(ErrorType::DIV_ZERO)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

// Edge cases with leading zeros
TEST_F(Bin2DecFunctionTest, LeadingZeros_HandlesCorrectly) {
    auto result = callBin2Dec({Value("0000000101")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(5.0, result.asNumber());
}