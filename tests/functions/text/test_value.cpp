#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class ValueFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callValue(const std::vector<Value>& args) {
        return builtin::value(args, context);
    }
};

TEST_F(ValueFunctionTest, NoArguments_ReturnsError) {
    auto result = callValue({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValueFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callValue({Value("123"), Value("456")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValueFunctionTest, EmptyString_ReturnsError) {
    auto result = callValue({Value("")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValueFunctionTest, WhitespaceOnly_ReturnsError) {
    auto result = callValue({Value("   ")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValueFunctionTest, BasicInteger_ConvertsCorrectly) {
    auto result = callValue({Value("123")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(123.0, result.asNumber());
}

TEST_F(ValueFunctionTest, BasicDecimal_ConvertsCorrectly) {
    auto result = callValue({Value("123.45")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(123.45, result.asNumber());
}

TEST_F(ValueFunctionTest, NegativeNumber_ConvertsCorrectly) {
    auto result = callValue({Value("-123.45")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-123.45, result.asNumber());
}

TEST_F(ValueFunctionTest, PositiveNumberWithPlus_ConvertsCorrectly) {
    auto result = callValue({Value("+123.45")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(123.45, result.asNumber());
}

TEST_F(ValueFunctionTest, NumberWithSpaces_TrimsSpaces) {
    auto result = callValue({Value("  123.45  ")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(123.45, result.asNumber());
}

TEST_F(ValueFunctionTest, Percentage_ConvertsCorrectly) {
    auto result = callValue({Value("12.34%")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.1234, result.asNumber());
}

TEST_F(ValueFunctionTest, PercentageWithSpaces_ConvertsCorrectly) {
    auto result = callValue({Value("  12.34%  ")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.1234, result.asNumber());
}

TEST_F(ValueFunctionTest, Currency_ConvertsCorrectly) {
    auto result = callValue({Value("$123.45")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(123.45, result.asNumber());
}

TEST_F(ValueFunctionTest, CurrencyWithSpaces_ConvertsCorrectly) {
    auto result = callValue({Value("  $123.45  ")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(123.45, result.asNumber());
}

TEST_F(ValueFunctionTest, CurrencyAndPercentage_ConvertsCorrectly) {
    auto result = callValue({Value("$12.34%")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.1234, result.asNumber());
}

TEST_F(ValueFunctionTest, Zero_ConvertsCorrectly) {
    auto result = callValue({Value("0")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ValueFunctionTest, ZeroDecimal_ConvertsCorrectly) {
    auto result = callValue({Value("0.0")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ValueFunctionTest, ScientificNotation_ConvertsCorrectly) {
    auto result = callValue({Value("1.23e2")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(123.0, result.asNumber());
}

TEST_F(ValueFunctionTest, ScientificNotationNegative_ConvertsCorrectly) {
    auto result = callValue({Value("1.23e-2")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0123, result.asNumber());
}

TEST_F(ValueFunctionTest, NonNumericText_ReturnsError) {
    auto result = callValue({Value("abc")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValueFunctionTest, MixedTextAndNumbers_ReturnsError) {
    auto result = callValue({Value("123abc")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValueFunctionTest, NumbersAndText_ReturnsError) {
    auto result = callValue({Value("abc123")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValueFunctionTest, SpecialCharacters_ReturnsError) {
    auto result = callValue({Value("!@#$%")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValueFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callValue({Value(123.45)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(123.45, result.asNumber());
}

TEST_F(ValueFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callValue({Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(ValueFunctionTest, BooleanFalseInput_ConvertsToTextFirst) {
    auto result = callValue({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ValueFunctionTest, ErrorInput_PropagatesError) {
    auto result = callValue({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ValueFunctionTest, LargeNumber_ConvertsCorrectly) {
    auto result = callValue({Value("1234567890.123")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1234567890.123, result.asNumber());
}

TEST_F(ValueFunctionTest, SmallDecimal_ConvertsCorrectly) {
    auto result = callValue({Value("0.000001")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.000001, result.asNumber());
}

TEST_F(ValueFunctionTest, PercentageZero_ConvertsCorrectly) {
    auto result = callValue({Value("0%")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ValueFunctionTest, PercentageHundred_ConvertsCorrectly) {
    auto result = callValue({Value("100%")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(ValueFunctionTest, CurrencyZero_ConvertsCorrectly) {
    auto result = callValue({Value("$0")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ValueFunctionTest, CurrencyNegative_ConvertsCorrectly) {
    auto result = callValue({Value("-$123.45")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-123.45, result.asNumber());
}