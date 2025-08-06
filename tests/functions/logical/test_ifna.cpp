#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class IfNaFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callIfNa(const std::vector<Value>& args) {
        return builtin::ifna_function(args, context);
    }
};

TEST_F(IfNaFunctionTest, NoArguments_ReturnsError) {
    auto result = callIfNa({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfNaFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callIfNa({Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfNaFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callIfNa({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfNaFunctionTest, NonNaErrorValue_ReturnsFirstArgument) {
    auto result = callIfNa({Value(42.0), Value("na")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(IfNaFunctionTest, NaErrorValue_ReturnsSecondArgument) {
    auto result = callIfNa({Value::error(ErrorType::NA_ERROR), Value("fallback")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("fallback", result.asText());
}

TEST_F(IfNaFunctionTest, OtherErrorTypes_ReturnsFirstArgument) {
    auto result = callIfNa({Value::error(ErrorType::VALUE_ERROR), Value("fallback")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfNaFunctionTest, TextValue_ReturnsFirstArgument) {
    auto result = callIfNa({Value("hello"), Value("na")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(IfNaFunctionTest, BooleanValue_ReturnsFirstArgument) {
    auto result = callIfNa({Value(true), Value("na")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IfNaFunctionTest, EmptyValue_ReturnsFirstArgument) {
    auto result = callIfNa({Value::empty(), Value("na")});

    EXPECT_TRUE(result.isEmpty());
}

TEST_F(IfNaFunctionTest, NumberAsFallback_ReturnsNumber) {
    auto result = callIfNa({Value::error(ErrorType::NA_ERROR), Value(100.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(100.0, result.asNumber());
}

TEST_F(IfNaFunctionTest, BooleanAsFallback_ReturnsBoolean) {
    auto result = callIfNa({Value::error(ErrorType::NA_ERROR), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IfNaFunctionTest, EmptyAsFallback_ReturnsEmpty) {
    auto result = callIfNa({Value::error(ErrorType::NA_ERROR), Value::empty()});

    EXPECT_TRUE(result.isEmpty());
}

TEST_F(IfNaFunctionTest, DifferentErrorTypes_DoNotTriggerFallback) {
    std::vector<ErrorType> non_na_error_types = {
        ErrorType::DIV_ZERO,
        ErrorType::VALUE_ERROR,
        ErrorType::REF_ERROR,
        ErrorType::NAME_ERROR,
        ErrorType::NUM_ERROR,
        ErrorType::PARSE_ERROR
    };

    for (const auto& error_type : non_na_error_types) {
        auto result = callIfNa({Value::error(error_type), Value("fallback")});
        EXPECT_TRUE(result.isError());
        EXPECT_EQ(error_type, result.asError());
    }
}

TEST_F(IfNaFunctionTest, ComplexValues_WorkCorrectly) {
    auto result = callIfNa({Value("complex text"), Value(42.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("complex text", result.asText());
}

TEST_F(IfNaFunctionTest, ErrorInFallback_StillReturnsFallback) {
    auto result = callIfNa({Value::error(ErrorType::NA_ERROR), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(IfNaFunctionTest, ZeroNumber_ReturnsFirstArgument) {
    auto result = callIfNa({Value(0.0), Value("na")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(IfNaFunctionTest, NegativeNumber_ReturnsFirstArgument) {
    auto result = callIfNa({Value(-5.0), Value("na")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-5.0, result.asNumber());
}

TEST_F(IfNaFunctionTest, NaErrorWithTextFallback_ReturnsText) {
    auto result = callIfNa({Value::error(ErrorType::NA_ERROR), Value("Not Available")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Not Available", result.asText());
}

TEST_F(IfNaFunctionTest, NaErrorWithNumberFallback_ReturnsNumber) {
    auto result = callIfNa({Value::error(ErrorType::NA_ERROR), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}