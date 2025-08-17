#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class SwitchFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callSwitch(const std::vector<Value>& args) {
        return builtin::switch_function(args, context);
    }
};

TEST_F(SwitchFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callSwitch({});
    EXPECT_TRUE(result.isError());

    result = callSwitch({Value(1.0)});
    EXPECT_TRUE(result.isError());

    result = callSwitch({Value(1.0), Value(1.0)});
    EXPECT_TRUE(result.isError());
}

TEST_F(SwitchFunctionTest, BasicMatch_ReturnsCorrectResult) {
    auto result = callSwitch({Value(2.0), Value(1.0), Value("One"), Value(2.0), Value("Two"),
                              Value(3.0), Value("Three")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Two", result.asText());
}

TEST_F(SwitchFunctionTest, FirstMatch_ReturnsFirstResult) {
    auto result = callSwitch({Value(1.0), Value(1.0), Value("First"), Value(1.0), Value("Second")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("First", result.asText());
}

TEST_F(SwitchFunctionTest, NoMatch_WithDefault_ReturnsDefault) {
    auto result = callSwitch(
            {Value(4.0), Value(1.0), Value("One"), Value(2.0), Value("Two"), Value("Default")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Default", result.asText());
}

TEST_F(SwitchFunctionTest, NoMatch_WithoutDefault_ReturnsNA) {
    auto result = callSwitch({Value(4.0), Value(1.0), Value("One"), Value(2.0), Value("Two")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NA_ERROR, result.asError());
}

TEST_F(SwitchFunctionTest, TextMatch_CaseSensitive) {
    auto result = callSwitch(
            {Value("hello"), Value("hello"), Value("Match"), Value("Hello"), Value("NoMatch")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Match", result.asText());

    result = callSwitch(
            {Value("Hello"), Value("hello"), Value("Match"), Value("Hello"), Value("NoMatch")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("NoMatch", result.asText());
}

TEST_F(SwitchFunctionTest, BooleanMatch_ReturnsCorrectResult) {
    auto result =
            callSwitch({Value(true), Value(false), Value("False"), Value(true), Value("True")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("True", result.asText());
}

TEST_F(SwitchFunctionTest, ExactTypeMatching_RequiredForMatch) {
    // Number 1 should not match text "1"
    auto result = callSwitch({Value(1.0), Value("1"), Value("Text"), Value(1.0), Value("Number")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Number", result.asText());
}

TEST_F(SwitchFunctionTest, ErrorInput_PropagatesError) {
    auto result = callSwitch({Value::error(ErrorType::VALUE_ERROR), Value(1.0), Value("One")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SwitchFunctionTest, ErrorInValues_PropagatesOnMatch) {
    auto result = callSwitch({Value(1.0), Value(1.0), Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SwitchFunctionTest, ErrorMatching_WithNonErrorValues_WorksCorrectly) {
    // Test error matching with non-error test values - should return default
    auto result = callSwitch(
            {Value::error(ErrorType::DIV_ZERO), Value(1.0), Value("One"), Value("Default")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Default", result.asText());
}

TEST_F(SwitchFunctionTest, FloatingPointMatch_WorksCorrectly) {
    auto result = callSwitch({Value(3.14), Value(3.14), Value("Pi"), Value(2.71), Value("E")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Pi", result.asText());
}

TEST_F(SwitchFunctionTest, EmptyTextMatch_WorksCorrectly) {
    auto result =
            callSwitch({Value(""), Value(""), Value("Empty"), Value("text"), Value("NotEmpty")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Empty", result.asText());
}

TEST_F(SwitchFunctionTest, ZeroMatch_WorksCorrectly) {
    auto result = callSwitch({Value(0.0), Value(0.0), Value("Zero"), Value(1.0), Value("One")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Zero", result.asText());
}

TEST_F(SwitchFunctionTest, MultipleValuePairs_FindsCorrectMatch) {
    auto result = callSwitch({Value("B"), Value("A"), Value("First"), Value("B"), Value("Second"),
                              Value("C"), Value("Third"), Value("Default")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Second", result.asText());
}

TEST_F(SwitchFunctionTest, ComplexResultTypes_ReturnsCorrectType) {
    // Test returning numbers
    auto result = callSwitch({Value(1.0), Value(1.0), Value(100.0), Value(2.0), Value(200.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(100.0, result.asNumber());

    // Test returning booleans
    result = callSwitch({Value(1.0), Value(1.0), Value(true), Value(2.0), Value(false)});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_EQ(true, result.asBoolean());
}

TEST_F(SwitchFunctionTest, LargeNumberOfPairs_WorksCorrectly) {
    std::vector<Value> args = {Value(5.0)};
    for (int i = 1; i <= 10; ++i) {
        args.push_back(Value(static_cast<double>(i)));
        args.push_back(Value("Result" + std::to_string(i)));
    }

    auto result = callSwitch(args);
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Result5", result.asText());
}
