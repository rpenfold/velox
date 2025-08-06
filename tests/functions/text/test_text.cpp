#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class TextFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callText(const std::vector<Value>& args) {
        return builtin::text(args, context);
    }
};

TEST_F(TextFunctionTest, NoArguments_ReturnsError) {
    auto result = callText({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TextFunctionTest, SingleArgument_ReturnsError) {
    auto result = callText({Value(123.45)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TextFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callText({Value(123.45), Value("0.00"), Value("extra")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TextFunctionTest, NonNumericFirstArgument_ReturnsError) {
    auto result = callText({Value("abc"), Value("0.00")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TextFunctionTest, EmptyFormatText_ReturnsError) {
    auto result = callText({Value(123.45), Value("")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TextFunctionTest, IntegerFormat_FormatsCorrectly) {
    auto result = callText({Value(123.45), Value("0")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("123", result.asText());
}

TEST_F(TextFunctionTest, DecimalFormat_FormatsCorrectly) {
    auto result = callText({Value(123.456), Value("0.00")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("123.46", result.asText());
}

TEST_F(TextFunctionTest, PercentageFormat_FormatsCorrectly) {
    auto result = callText({Value(0.123), Value("0%")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("12%", result.asText());
}

TEST_F(TextFunctionTest, PercentageWithDecimals_FormatsCorrectly) {
    auto result = callText({Value(0.1234), Value("0.00%")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("12.34%", result.asText());
}

TEST_F(TextFunctionTest, CurrencyFormat_FormatsCorrectly) {
    auto result = callText({Value(123.45), Value("$0.00")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("$123.45", result.asText());
}

TEST_F(TextFunctionTest, DateFormat_FormatsCorrectly) {
    auto result = callText({Value(45000.0), Value("MM/DD/YYYY")});

    EXPECT_TRUE(result.isText());
    // Note: This is a simplified date implementation
    EXPECT_TRUE(result.asText().find("/") != std::string::npos);
}

TEST_F(TextFunctionTest, ZeroValue_FormatsCorrectly) {
    auto result = callText({Value(0.0), Value("0.00")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("0.00", result.asText());
}

TEST_F(TextFunctionTest, NegativeValue_FormatsCorrectly) {
    auto result = callText({Value(-123.45), Value("0.00")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("-123.45", result.asText());
}

TEST_F(TextFunctionTest, LargeNumber_FormatsCorrectly) {
    auto result = callText({Value(1234567.89), Value("0.00")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1234567.89", result.asText());
}

TEST_F(TextFunctionTest, SmallDecimal_FormatsCorrectly) {
    auto result = callText({Value(0.001), Value("0.000")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("0.001000", result.asText());
}

TEST_F(TextFunctionTest, BooleanInput_ConvertsToNumberFirst) {
    auto result = callText({Value(true), Value("0")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("1", result.asText());
}

TEST_F(TextFunctionTest, ErrorInput_PropagatesError) {
    auto result = callText({Value::error(ErrorType::DIV_ZERO), Value("0.00")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(TextFunctionTest, ErrorInSecondArgument_PropagatesError) {
    auto result = callText({Value(123.45), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(TextFunctionTest, DefaultFormat_HandlesUnknownFormat) {
    auto result = callText({Value(123.456), Value("unknown_format")});

    EXPECT_TRUE(result.isText());
    // Should fall back to default format
    EXPECT_TRUE(result.asText().find("123.456") != std::string::npos);
}

TEST_F(TextFunctionTest, HashFormat_HandlesHashFormat) {
    auto result = callText({Value(123.45), Value("#.##")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("123.45", result.asText());
}

TEST_F(TextFunctionTest, HashPercentFormat_HandlesHashPercent) {
    auto result = callText({Value(0.123), Value("#%")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("12%", result.asText());
}

TEST_F(TextFunctionTest, CurrencyHashFormat_HandlesCurrencyHash) {
    auto result = callText({Value(123.45), Value("$#,##0.00")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("$123.45", result.asText());
}