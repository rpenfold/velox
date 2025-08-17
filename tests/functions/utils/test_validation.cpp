#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class ValidationUtilsTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // No setup needed for utility function tests
    }
};

// validateMinArgs Tests
TEST_F(ValidationUtilsTest, ValidateMinArgs_EnoughArguments_ReturnsEmpty) {
    std::vector<Value> args = {Value(1.0), Value(2.0)};
    auto result = utils::validateMinArgs(args, 2, "TEST");

    EXPECT_TRUE(result.isEmpty());
}

TEST_F(ValidationUtilsTest, ValidateMinArgs_MoreThanEnough_ReturnsEmpty) {
    std::vector<Value> args = {Value(1.0), Value(2.0), Value(3.0)};
    auto result = utils::validateMinArgs(args, 2, "TEST");

    EXPECT_TRUE(result.isEmpty());
}

TEST_F(ValidationUtilsTest, ValidateMinArgs_TooFewArguments_ReturnsError) {
    std::vector<Value> args = {Value(1.0)};
    auto result = utils::validateMinArgs(args, 2, "TEST");

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValidationUtilsTest, ValidateMinArgs_EmptyArgs_ReturnsError) {
    std::vector<Value> args = {};
    auto result = utils::validateMinArgs(args, 1, "TEST");

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValidationUtilsTest, ValidateMinArgs_ZeroRequired_AlwaysSuccess) {
    std::vector<Value> args = {};
    auto result = utils::validateMinArgs(args, 0, "TEST");

    EXPECT_TRUE(result.isEmpty());
}

// validateArgCount Tests
TEST_F(ValidationUtilsTest, ValidateArgCount_ExactMatch_ReturnsEmpty) {
    std::vector<Value> args = {Value(1.0), Value(2.0)};
    auto result = utils::validateArgCount(args, 2, "TEST");

    EXPECT_TRUE(result.isEmpty());
}

TEST_F(ValidationUtilsTest, ValidateArgCount_TooMany_ReturnsError) {
    std::vector<Value> args = {Value(1.0), Value(2.0), Value(3.0)};
    auto result = utils::validateArgCount(args, 2, "TEST");

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValidationUtilsTest, ValidateArgCount_TooFew_ReturnsError) {
    std::vector<Value> args = {Value(1.0)};
    auto result = utils::validateArgCount(args, 2, "TEST");

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValidationUtilsTest, ValidateArgCount_EmptyArgs_ZeroRequired_Success) {
    std::vector<Value> args = {};
    auto result = utils::validateArgCount(args, 0, "TEST");

    EXPECT_TRUE(result.isEmpty());
}

TEST_F(ValidationUtilsTest, ValidateArgCount_EmptyArgs_OneRequired_Error) {
    std::vector<Value> args = {};
    auto result = utils::validateArgCount(args, 1, "TEST");

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// checkForErrors Tests
TEST_F(ValidationUtilsTest, CheckForErrors_NoErrors_ReturnsEmpty) {
    std::vector<Value> args = {Value(1.0), Value(2.0)};
    auto result = utils::checkForErrors(args);

    EXPECT_TRUE(result.isEmpty());
}

TEST_F(ValidationUtilsTest, CheckForErrors_WithError_ReturnsFirstError) {
    std::vector<Value> args = {Value(1.0), Value::error(ErrorType::DIV_ZERO), Value(2.0)};
    auto result = utils::checkForErrors(args);

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ValidationUtilsTest, CheckForErrors_MultipleErrors_ReturnsFirst) {
    std::vector<Value> args = {Value::error(ErrorType::DIV_ZERO),
                               Value::error(ErrorType::VALUE_ERROR)};
    auto result = utils::checkForErrors(args);

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ValidationUtilsTest, CheckForErrors_EmptyArgs_ReturnsEmpty) {
    std::vector<Value> args = {};
    auto result = utils::checkForErrors(args);

    EXPECT_TRUE(result.isEmpty());
}

TEST_F(ValidationUtilsTest, CheckForErrors_MixedTypes_OnlyErrorsDetected) {
    std::vector<Value> args = {Value(1.0), Value("text"), Value(true),
                               Value::error(ErrorType::REF_ERROR), Value(false)};
    auto result = utils::checkForErrors(args);

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

// toNumberSafe Tests
TEST_F(ValidationUtilsTest, ToNumberSafe_ValidNumber_ReturnsNumber) {
    auto result = utils::toNumberSafe(Value(42.0), "TEST");

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(ValidationUtilsTest, ToNumberSafe_ValidBoolean_ConvertsToNumber) {
    auto result = utils::toNumberSafe(Value(true), "TEST");

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(ValidationUtilsTest, ToNumberSafe_FalseBoolean_ConvertsToZero) {
    auto result = utils::toNumberSafe(Value(false), "TEST");

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ValidationUtilsTest, ToNumberSafe_NumericText_ConvertsToNumber) {
    auto result = utils::toNumberSafe(Value("123.45"), "TEST");

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(123.45, result.asNumber());
}

TEST_F(ValidationUtilsTest, ToNumberSafe_NonNumericText_ReturnsError) {
    auto result = utils::toNumberSafe(Value("hello"), "TEST");

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValidationUtilsTest, ToNumberSafe_ErrorInput_PropagatesError) {
    auto result = utils::toNumberSafe(Value::error(ErrorType::DIV_ZERO), "TEST");

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ValidationUtilsTest, ToNumberSafe_EmptyValue_ReturnsError) {
    auto result = utils::toNumberSafe(Value::empty(), "TEST");

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ValidationUtilsTest, ToNumberSafe_NegativeNumber_ReturnsCorrectValue) {
    auto result = utils::toNumberSafe(Value(-42.5), "TEST");

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-42.5, result.asNumber());
}

TEST_F(ValidationUtilsTest, ToNumberSafe_Zero_ReturnsZero) {
    auto result = utils::toNumberSafe(Value(0.0), "TEST");

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}