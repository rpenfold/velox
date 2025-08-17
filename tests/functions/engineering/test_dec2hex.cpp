#include <gtest/gtest.h>
#include "velox/formulas/functions.h"
#include "velox/formulas/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class Dec2HexFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callDec2Hex(const std::vector<Value>& args) {
        return dec2hex(args, context);
    }
};

// Basic functionality tests
TEST_F(Dec2HexFunctionTest, NoArguments_ReturnsError) {
    auto result = callDec2Hex({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(Dec2HexFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callDec2Hex({Value(10.0), Value(2.0), Value(3.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Valid conversions
TEST_F(Dec2HexFunctionTest, Zero_ReturnsZero) {
    auto result = callDec2Hex({Value(0.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());
}

TEST_F(Dec2HexFunctionTest, SingleDigit_ConvertsCorrectly) {
    auto result = callDec2Hex({Value(10.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(Dec2HexFunctionTest, MultipleDigits_ConvertsCorrectly) {
    auto result = callDec2Hex({Value(255.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("FF", result.asText());
}

TEST_F(Dec2HexFunctionTest, LargeNumber_ConvertsCorrectly) {
    auto result = callDec2Hex({Value(4096.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1000", result.asText());
}

// With places parameter
TEST_F(Dec2HexFunctionTest, WithPlaces_PadsCorrectly) {
    auto result = callDec2Hex({Value(10.0), Value(4.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("000A", result.asText());
}

TEST_F(Dec2HexFunctionTest, PlacesLessThanNeeded_NoTruncation) {
    auto result = callDec2Hex({Value(255.0), Value(1.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("FF", result.asText());
}

TEST_F(Dec2HexFunctionTest, PlacesZero_NoPadding) {
    auto result = callDec2Hex({Value(10.0), Value(0.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

// Negative numbers
TEST_F(Dec2HexFunctionTest, NegativeOne_ConvertsCorrectly) {
    auto result = callDec2Hex({Value(-1.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("FFFFFFFFFF", result.asText());
}

TEST_F(Dec2HexFunctionTest, NegativeTwo_ConvertsCorrectly) {
    auto result = callDec2Hex({Value(-2.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("FFFFFFFFFE", result.asText());
}

TEST_F(Dec2HexFunctionTest, LargeNegative_ConvertsCorrectly) {
    auto result = callDec2Hex({Value(-512.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("FFFFFE00", result.asText());
}

// Error cases
TEST_F(Dec2HexFunctionTest, OutOfRangePositive_ReturnsError) {
    auto result = callDec2Hex({Value(549755813888.0)});  // Too large
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Dec2HexFunctionTest, OutOfRangeNegative_ReturnsError) {
    auto result = callDec2Hex({Value(-549755813889.0)});  // Too small
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(Dec2HexFunctionTest, NegativePlaces_ReturnsError) {
    auto result = callDec2Hex({Value(10.0), Value(-1.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// Type conversions
TEST_F(Dec2HexFunctionTest, FloatingPoint_TruncatesInteger) {
    auto result = callDec2Hex({Value(10.7)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());
}

TEST_F(Dec2HexFunctionTest, TextInput_ConvertsToNumber) {
    auto result = callDec2Hex({Value("255")});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("FF", result.asText());
}

TEST_F(Dec2HexFunctionTest, BooleanTrue_ConvertsToOne) {
    auto result = callDec2Hex({Value(true)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1", result.asText());
}

TEST_F(Dec2HexFunctionTest, BooleanFalse_ConvertsToZero) {
    auto result = callDec2Hex({Value(false)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());
}

// Error propagation
TEST_F(Dec2HexFunctionTest, ErrorInput_PropagatesError) {
    auto result = callDec2Hex({Value::error(ErrorType::DIV_ZERO)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(Dec2HexFunctionTest, NonNumericText_ReturnsError) {
    auto result = callDec2Hex({Value("abc")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Edge cases
TEST_F(Dec2HexFunctionTest, MaxPositive_ConvertsCorrectly) {
    auto result = callDec2Hex({Value(549755813887.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("7FFFFFFFFF", result.asText());
}

TEST_F(Dec2HexFunctionTest, MaxNegative_ConvertsCorrectly) {
    auto result = callDec2Hex({Value(-549755813888.0)});
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("8000000000", result.asText());
}