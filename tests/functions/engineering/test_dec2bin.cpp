#include <gtest/gtest.h>
#include "velox/formulas/functions.h"
#include "velox/formulas/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class Dec2BinFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callDec2Bin(const std::vector<Value>& args) {
        return dec2bin(args, context);
    }
};

// Basic functionality tests
TEST_F(Dec2BinFunctionTest, NoArguments_ReturnsError) {
    auto result = callDec2Bin({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(Dec2BinFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callDec2Bin({Value(5.0), Value(4.0), Value(3.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Valid conversions
TEST_F(Dec2BinFunctionTest, Zero_ReturnsZero) {
    auto result = callDec2Bin({Value(0.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());
}

TEST_F(Dec2BinFunctionTest, One_ReturnsOne) {
    auto result = callDec2Bin({Value(1.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1", result.asText());
}

TEST_F(Dec2BinFunctionTest, Five_ConvertsCorrectly) {
    auto result = callDec2Bin({Value(5.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("101", result.asText());
}

TEST_F(Dec2BinFunctionTest, Fifteen_ConvertsCorrectly) {
    auto result = callDec2Bin({Value(15.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1111", result.asText());
}

TEST_F(Dec2BinFunctionTest, MaxPositive_ConvertsCorrectly) {
    auto result = callDec2Bin({Value(511.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("111111111", result.asText());
}

// With places parameter
TEST_F(Dec2BinFunctionTest, WithPlaces_PadsCorrectly) {
    auto result = callDec2Bin({Value(5.0), Value(8.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("00000101", result.asText());
}

TEST_F(Dec2BinFunctionTest, PlacesLessThanNeeded_NoTruncation) {
    auto result = callDec2Bin({Value(15.0), Value(2.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1111", result.asText());
}

TEST_F(Dec2BinFunctionTest, PlacesZero_NoPadding) {
    auto result = callDec2Bin({Value(5.0), Value(0.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("101", result.asText());
}

// Negative numbers (two's complement)
TEST_F(Dec2BinFunctionTest, NegativeOne_ConvertsCorrectly) {
    auto result = callDec2Bin({Value(-1.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1111111111", result.asText());
}

TEST_F(Dec2BinFunctionTest, NegativeTwo_ConvertsCorrectly) {
    auto result = callDec2Bin({Value(-2.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1111111110", result.asText());
}

TEST_F(Dec2BinFunctionTest, MaxNegative_ConvertsCorrectly) {
    auto result = callDec2Bin({Value(-512.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1000000000", result.asText());
}

// Error cases
TEST_F(Dec2BinFunctionTest, OutOfRangePositive_ReturnsError) {
    auto result = callDec2Bin({Value(512.0)});  // Too large
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Dec2BinFunctionTest, OutOfRangeNegative_ReturnsError) {
    auto result = callDec2Bin({Value(-513.0)});  // Too small
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Dec2BinFunctionTest, NegativePlaces_ReturnsError) {
    auto result = callDec2Bin({Value(5.0), Value(-1.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// Type conversions
TEST_F(Dec2BinFunctionTest, FloatingPoint_TruncatesInteger) {
    auto result = callDec2Bin({Value(5.7)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("101", result.asText());
}

TEST_F(Dec2BinFunctionTest, TextInput_ConvertsToNumber) {
    auto result = callDec2Bin({Value("15")});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1111", result.asText());
}

TEST_F(Dec2BinFunctionTest, BooleanTrue_ConvertsToOne) {
    auto result = callDec2Bin({Value(true)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1", result.asText());
}

TEST_F(Dec2BinFunctionTest, BooleanFalse_ConvertsToZero) {
    auto result = callDec2Bin({Value(false)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());
}

// Error propagation
TEST_F(Dec2BinFunctionTest, ErrorInput_PropagatesError) {
    auto result = callDec2Bin({Value::error(ErrorType::DIV_ZERO)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(Dec2BinFunctionTest, NonNumericText_ReturnsError) {
    auto result = callDec2Bin({Value("abc")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Edge cases with padding
TEST_F(Dec2BinFunctionTest, LargePlaces_PadsCorrectly) {
    auto result = callDec2Bin({Value(1.0), Value(10.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("0000000001", result.asText());
}