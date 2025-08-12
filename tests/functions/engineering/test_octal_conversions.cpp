#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class OctalConversionsTest : public ::testing::Test {
  protected:
    Context context;

    Value callDec2Oct(const std::vector<Value>& args) {
        return builtin::dec2oct(args, context);
    }

    Value callBin2Oct(const std::vector<Value>& args) {
        return builtin::bin2oct(args, context);
    }

    Value callOct2Bin(const std::vector<Value>& args) {
        return builtin::oct2bin(args, context);
    }

    Value callHex2Oct(const std::vector<Value>& args) {
        return builtin::hex2oct(args, context);
    }

    Value callOct2Hex(const std::vector<Value>& args) {
        return builtin::oct2hex(args, context);
    }
};

// DEC2OCT Tests
TEST_F(OctalConversionsTest, Dec2Oct_BasicConversion_WorksCorrectly) {
    auto result = callDec2Oct({Value(8.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("10", result.asText());  // 8 decimal = 10 octal

    result = callDec2Oct({Value(64.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("100", result.asText());  // 64 decimal = 100 octal
}

TEST_F(OctalConversionsTest, Dec2Oct_ZeroValue_ReturnsZero) {
    auto result = callDec2Oct({Value(0.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());
}

TEST_F(OctalConversionsTest, Dec2Oct_WithPlaces_AddsPadding) {
    auto result = callDec2Oct({Value(8.0), Value(4.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("0010", result.asText());  // Padded to 4 places
}

TEST_F(OctalConversionsTest, Dec2Oct_TooFewArguments_ReturnsError) {
    auto result = callDec2Oct({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// BIN2OCT Tests
TEST_F(OctalConversionsTest, Bin2Oct_BasicConversion_WorksCorrectly) {
    auto result = callBin2Oct({Value("1000")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("10", result.asText());  // 1000 binary = 8 decimal = 10 octal

    result = callBin2Oct({Value("1010")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("12", result.asText());  // 1010 binary = 10 decimal = 12 octal
}

TEST_F(OctalConversionsTest, Bin2Oct_ZeroValue_ReturnsZero) {
    auto result = callBin2Oct({Value("0")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());
}

TEST_F(OctalConversionsTest, Bin2Oct_InvalidBinary_ReturnsError) {
    auto result = callBin2Oct({Value("102")});  // Invalid binary (contains 2)
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(OctalConversionsTest, Bin2Oct_TooLong_ReturnsError) {
    auto result = callBin2Oct({Value("12345678901")});  // Too many digits
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// OCT2BIN Tests
TEST_F(OctalConversionsTest, Oct2Bin_BasicConversion_WorksCorrectly) {
    auto result = callOct2Bin({Value("10")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("1000", result.asText());  // 10 octal = 8 decimal = 1000 binary

    result = callOct2Bin({Value("12")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("1010", result.asText());  // 12 octal = 10 decimal = 1010 binary
}

TEST_F(OctalConversionsTest, Oct2Bin_ZeroValue_ReturnsZero) {
    auto result = callOct2Bin({Value("0")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());
}

TEST_F(OctalConversionsTest, Oct2Bin_InvalidOctal_ReturnsError) {
    auto result = callOct2Bin({Value("89")});  // Invalid octal (contains 8 and 9)
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// HEX2OCT Tests
TEST_F(OctalConversionsTest, Hex2Oct_BasicConversion_WorksCorrectly) {
    auto result = callHex2Oct({Value("8")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("10", result.asText());  // 8 hex = 8 decimal = 10 octal

    result = callHex2Oct({Value("A")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("12", result.asText());  // A hex = 10 decimal = 12 octal
}

TEST_F(OctalConversionsTest, Hex2Oct_ZeroValue_ReturnsZero) {
    auto result = callHex2Oct({Value("0")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());
}

TEST_F(OctalConversionsTest, Hex2Oct_LowercaseHex_WorksCorrectly) {
    auto result = callHex2Oct({Value("a")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("12", result.asText());  // Should work with lowercase
}

TEST_F(OctalConversionsTest, Hex2Oct_InvalidHex_ReturnsError) {
    auto result = callHex2Oct({Value("G")});  // Invalid hex character
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// OCT2HEX Tests
TEST_F(OctalConversionsTest, Oct2Hex_BasicConversion_WorksCorrectly) {
    auto result = callOct2Hex({Value("10")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("8", result.asText());  // 10 octal = 8 decimal = 8 hex

    result = callOct2Hex({Value("12")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("A", result.asText());  // 12 octal = 10 decimal = A hex
}

TEST_F(OctalConversionsTest, Oct2Hex_ZeroValue_ReturnsZero) {
    auto result = callOct2Hex({Value("0")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());
}

TEST_F(OctalConversionsTest, Oct2Hex_LargerValues_WorksCorrectly) {
    auto result = callOct2Hex({Value("100")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("40", result.asText());  // 100 octal = 64 decimal = 40 hex
}

TEST_F(OctalConversionsTest, Oct2Hex_InvalidOctal_ReturnsError) {
    auto result = callOct2Hex({Value("89")});  // Invalid octal
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// Error propagation tests
TEST_F(OctalConversionsTest, AllFunctions_ErrorInput_PropagatesError) {
    auto result = callDec2Oct({Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());

    result = callBin2Oct({Value::error(ErrorType::DIV_ZERO)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());

    result = callOct2Bin({Value::error(ErrorType::NUM_ERROR)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}
