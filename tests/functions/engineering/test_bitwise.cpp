#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class BitwiseFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callBitAnd(const std::vector<Value>& args) {
        return bitand_function(args, context);
    }

    Value callBitOr(const std::vector<Value>& args) {
        return bitor_function(args, context);
    }

    Value callBitXor(const std::vector<Value>& args) {
        return bitxor_function(args, context);
    }
};

// BITAND Tests
TEST_F(BitwiseFunctionTest, BitAnd_NoArguments_ReturnsError) {
    auto result = callBitAnd({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(BitwiseFunctionTest, BitAnd_OneArgument_ReturnsError) {
    auto result = callBitAnd({Value(5.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(BitwiseFunctionTest, BitAnd_TooManyArguments_ReturnsError) {
    auto result = callBitAnd({Value(5.0), Value(3.0), Value(1.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(BitwiseFunctionTest, BitAnd_BasicOperation_WorksCorrectly) {
    auto result = callBitAnd({Value(5.0), Value(3.0)});  // 101 & 011 = 001
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(1.0, result.asNumber());
}

TEST_F(BitwiseFunctionTest, BitAnd_AllOnes_ReturnsInput) {
    auto result = callBitAnd({Value(15.0), Value(15.0)});  // 1111 & 1111 = 1111
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(15.0, result.asNumber());
}

TEST_F(BitwiseFunctionTest, BitAnd_WithZero_ReturnsZero) {
    auto result = callBitAnd({Value(255.0), Value(0.0)});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(0.0, result.asNumber());
}

// BITOR Tests
TEST_F(BitwiseFunctionTest, BitOr_BasicOperation_WorksCorrectly) {
    auto result = callBitOr({Value(5.0), Value(3.0)});  // 101 | 011 = 111
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(7.0, result.asNumber());
}

TEST_F(BitwiseFunctionTest, BitOr_WithZero_ReturnsInput) {
    auto result = callBitOr({Value(15.0), Value(0.0)});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(15.0, result.asNumber());
}

TEST_F(BitwiseFunctionTest, BitOr_DisjointBits_AddsThem) {
    auto result = callBitOr({Value(8.0), Value(4.0)});  // 1000 | 0100 = 1100
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(12.0, result.asNumber());
}

// BITXOR Tests
TEST_F(BitwiseFunctionTest, BitXor_BasicOperation_WorksCorrectly) {
    auto result = callBitXor({Value(5.0), Value(3.0)});  // 101 ^ 011 = 110
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(6.0, result.asNumber());
}

TEST_F(BitwiseFunctionTest, BitXor_SameNumbers_ReturnsZero) {
    auto result = callBitXor({Value(15.0), Value(15.0)});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(0.0, result.asNumber());
}

TEST_F(BitwiseFunctionTest, BitXor_WithZero_ReturnsInput) {
    auto result = callBitXor({Value(255.0), Value(0.0)});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(255.0, result.asNumber());
}

// Range validation tests
TEST_F(BitwiseFunctionTest, BitAnd_NegativeNumber_ReturnsError) {
    auto result = callBitAnd({Value(-1.0), Value(5.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(BitwiseFunctionTest, BitOr_TooLarge_ReturnsError) {
    auto result = callBitOr({Value(281474976710656.0), Value(5.0)});  // Beyond 48-bit limit
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(BitwiseFunctionTest, BitXor_MaxValid_WorksCorrectly) {
    auto result = callBitXor({Value(281474976710655.0), Value(0.0)});  // Max 48-bit value
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(281474976710655.0, result.asNumber());
}

// Type conversion tests
TEST_F(BitwiseFunctionTest, BitAnd_FloatingPoint_TruncatesInteger) {
    auto result = callBitAnd({Value(5.7), Value(3.9)});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(1.0, result.asNumber());  // floor(5.7) & floor(3.9) = 5 & 3 = 1
}

TEST_F(BitwiseFunctionTest, BitOr_TextInput_ConvertsToNumber) {
    auto result = callBitOr({Value("5"), Value("3")});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(7.0, result.asNumber());
}

TEST_F(BitwiseFunctionTest, BitXor_BooleanInput_ConvertsToNumber) {
    auto result = callBitXor({Value(true), Value(false)});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(1.0, result.asNumber());  // 1 ^ 0 = 1
}

// Error propagation tests
TEST_F(BitwiseFunctionTest, BitAnd_ErrorInput_PropagatesError) {
    auto result = callBitAnd({Value::error(ErrorType::DIV_ZERO), Value(5.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(BitwiseFunctionTest, BitOr_NonNumericText_ReturnsError) {
    auto result = callBitOr({Value("abc"), Value(5.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(BitwiseFunctionTest, BitXor_SecondArgumentError_PropagatesError) {
    auto result = callBitXor({Value(5.0), Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Large number tests
TEST_F(BitwiseFunctionTest, BitAnd_LargeNumbers_WorksCorrectly) {
    auto result = callBitAnd(
            {Value(static_cast<double>(0xFFFFFFFFFF)), Value(static_cast<double>(0x123456789A))});
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(0x123456789A, result.asNumber());
}

TEST_F(BitwiseFunctionTest, BitOr_PowersOfTwo_WorksCorrectly) {
    auto result = callBitOr({Value(1024.0), Value(2048.0)});  // 2^10 | 2^11
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(3072.0, result.asNumber());  // 1024 + 2048
}

TEST_F(BitwiseFunctionTest, BitXor_AlternatingBits_WorksCorrectly) {
    auto result = callBitXor({Value(static_cast<double>(0xAAAAAAAAA)),
                              Value(static_cast<double>(0x555555555))});  // Alternating bits
    EXPECT_TRUE(result.isNumber());
    EXPECT_EQ(0xFFFFFFFFF, result.asNumber());  // All bits set
}