#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class IfErrorFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callIfError(const std::vector<Value>& args) {
        return builtin::iferror_function(args, context);
    }
};

TEST_F(IfErrorFunctionTest, NoArguments_ReturnsError) {
    auto result = callIfError({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfErrorFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callIfError({Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfErrorFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callIfError({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfErrorFunctionTest, NonErrorValue_ReturnsFirstArgument) {
    auto result = callIfError({Value(42.0), Value("error")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(IfErrorFunctionTest, ErrorValue_ReturnsSecondArgument) {
    auto result = callIfError({Value::error(ErrorType::VALUE_ERROR), Value("fallback")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("fallback", result.asText());
}

TEST_F(IfErrorFunctionTest, DifferentErrorTypes_ReturnsSecondArgument) {
    auto result = callIfError({Value::error(ErrorType::DIV_ZERO), Value("division error")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("division error", result.asText());
}

TEST_F(IfErrorFunctionTest, TextValue_ReturnsFirstArgument) {
    auto result = callIfError({Value("hello"), Value("error")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(IfErrorFunctionTest, BooleanValue_ReturnsFirstArgument) {
    auto result = callIfError({Value(true), Value("error")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IfErrorFunctionTest, EmptyValue_ReturnsFirstArgument) {
    auto result = callIfError({Value::empty(), Value("error")});

    EXPECT_TRUE(result.isEmpty());
}

TEST_F(IfErrorFunctionTest, NumberAsFallback_ReturnsNumber) {
    auto result = callIfError({Value::error(ErrorType::VALUE_ERROR), Value(100.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(100.0, result.asNumber());
}

TEST_F(IfErrorFunctionTest, BooleanAsFallback_ReturnsBoolean) {
    auto result = callIfError({Value::error(ErrorType::REF_ERROR), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IfErrorFunctionTest, EmptyAsFallback_ReturnsEmpty) {
    auto result = callIfError({Value::error(ErrorType::NAME_ERROR), Value::empty()});

    EXPECT_TRUE(result.isEmpty());
}

TEST_F(IfErrorFunctionTest, AllErrorTypes_WorkCorrectly) {
    std::vector<ErrorType> error_types = {ErrorType::DIV_ZERO,   ErrorType::VALUE_ERROR,
                                          ErrorType::REF_ERROR,  ErrorType::NAME_ERROR,
                                          ErrorType::NUM_ERROR,  ErrorType::NA_ERROR,
                                          ErrorType::PARSE_ERROR};

    for (const auto& error_type : error_types) {
        auto result = callIfError({Value::error(error_type), Value("fallback")});
        EXPECT_TRUE(result.isText());
        EXPECT_EQ("fallback", result.asText());
    }
}

TEST_F(IfErrorFunctionTest, ComplexValues_WorkCorrectly) {
    auto result = callIfError({Value("complex text"), Value(42.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("complex text", result.asText());
}

TEST_F(IfErrorFunctionTest, ErrorInFallback_StillReturnsFallback) {
    auto result =
            callIfError({Value::error(ErrorType::VALUE_ERROR), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(IfErrorFunctionTest, ZeroNumber_ReturnsFirstArgument) {
    auto result = callIfError({Value(0.0), Value("error")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(IfErrorFunctionTest, NegativeNumber_ReturnsFirstArgument) {
    auto result = callIfError({Value(-5.0), Value("error")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-5.0, result.asNumber());
}