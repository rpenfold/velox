#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class IsFunctionsTest : public ::testing::Test {
  protected:
    Context context;

    Value callIsNumber(const std::vector<Value>& args) {
        return builtin::isnumber_function(args, context);
    }

    Value callIsText(const std::vector<Value>& args) {
        return builtin::istext_function(args, context);
    }

    Value callIsBlank(const std::vector<Value>& args) {
        return builtin::isblank_function(args, context);
    }

    Value callIsError(const std::vector<Value>& args) {
        return builtin::iserror_function(args, context);
    }
};

// ISNUMBER Tests
TEST_F(IsFunctionsTest, IsNumber_NoArguments_ReturnsError) {
    auto result = callIsNumber({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IsFunctionsTest, IsNumber_TooManyArguments_ReturnsError) {
    auto result = callIsNumber({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IsFunctionsTest, IsNumber_NumberValue_ReturnsTrue) {
    auto result = callIsNumber({Value(42.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsNumber_TextNumber_ReturnsTrue) {
    auto result = callIsNumber({Value("42")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsNumber_TextValue_ReturnsFalse) {
    auto result = callIsNumber({Value("hello")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsNumber_BooleanValue_ReturnsTrue) {
    auto result = callIsNumber({Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsNumber_EmptyValue_ReturnsFalse) {
    auto result = callIsNumber({Value::empty()});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsNumber_ErrorValue_ReturnsFalse) {
    auto result = callIsNumber({Value::error(ErrorType::VALUE_ERROR)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsNumber_NegativeNumber_ReturnsTrue) {
    auto result = callIsNumber({Value(-5.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsNumber_ZeroNumber_ReturnsTrue) {
    auto result = callIsNumber({Value(0.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsNumber_DecimalNumber_ReturnsTrue) {
    auto result = callIsNumber({Value(3.14159)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

// ISTEXT Tests
TEST_F(IsFunctionsTest, IsText_NoArguments_ReturnsError) {
    auto result = callIsText({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IsFunctionsTest, IsText_TooManyArguments_ReturnsError) {
    auto result = callIsText({Value("hello"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IsFunctionsTest, IsText_TextValue_ReturnsTrue) {
    auto result = callIsText({Value("hello")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsText_NumberValue_ReturnsFalse) {
    auto result = callIsText({Value(42.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsText_BooleanValue_ReturnsFalse) {
    auto result = callIsText({Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsText_EmptyValue_ReturnsFalse) {
    auto result = callIsText({Value::empty()});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsText_ErrorValue_ReturnsFalse) {
    auto result = callIsText({Value::error(ErrorType::VALUE_ERROR)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsText_EmptyString_ReturnsTrue) {
    auto result = callIsText({Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsText_WhitespaceString_ReturnsTrue) {
    auto result = callIsText({Value(" ")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

// ISBLANK Tests
TEST_F(IsFunctionsTest, IsBlank_NoArguments_ReturnsError) {
    auto result = callIsBlank({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IsFunctionsTest, IsBlank_TooManyArguments_ReturnsError) {
    auto result = callIsBlank({Value::empty(), Value::empty()});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IsFunctionsTest, IsBlank_EmptyValue_ReturnsTrue) {
    auto result = callIsBlank({Value::empty()});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsBlank_NumberValue_ReturnsFalse) {
    auto result = callIsBlank({Value(42.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsBlank_TextValue_ReturnsFalse) {
    auto result = callIsBlank({Value("hello")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsBlank_BooleanValue_ReturnsFalse) {
    auto result = callIsBlank({Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsBlank_ErrorValue_ReturnsFalse) {
    auto result = callIsBlank({Value::error(ErrorType::VALUE_ERROR)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsBlank_EmptyString_ReturnsFalse) {
    auto result = callIsBlank({Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

// ISERROR Tests
TEST_F(IsFunctionsTest, IsError_NoArguments_ReturnsError) {
    auto result = callIsError({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IsFunctionsTest, IsError_TooManyArguments_ReturnsError) {
    auto result =
            callIsError({Value::error(ErrorType::VALUE_ERROR), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IsFunctionsTest, IsError_ErrorValue_ReturnsTrue) {
    auto result = callIsError({Value::error(ErrorType::VALUE_ERROR)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsError_NumberValue_ReturnsFalse) {
    auto result = callIsError({Value(42.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsError_TextValue_ReturnsFalse) {
    auto result = callIsError({Value("hello")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsError_BooleanValue_ReturnsFalse) {
    auto result = callIsError({Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsError_EmptyValue_ReturnsFalse) {
    auto result = callIsError({Value::empty()});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsError_AllErrorTypes_ReturnTrue) {
    std::vector<ErrorType> error_types = {ErrorType::DIV_ZERO,   ErrorType::VALUE_ERROR,
                                          ErrorType::REF_ERROR,  ErrorType::NAME_ERROR,
                                          ErrorType::NUM_ERROR,  ErrorType::NA_ERROR,
                                          ErrorType::PARSE_ERROR};

    for (const auto& error_type : error_types) {
        auto result = callIsError({Value::error(error_type)});
        EXPECT_TRUE(result.isBoolean());
        EXPECT_TRUE(result.asBoolean());
    }
}

TEST_F(IsFunctionsTest, IsError_ZeroNumber_ReturnsFalse) {
    auto result = callIsError({Value(0.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(IsFunctionsTest, IsError_NegativeNumber_ReturnsFalse) {
    auto result = callIsError({Value(-5.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}