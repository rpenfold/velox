#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class ConcatenateFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callConcatenate(const std::vector<Value>& args) {
        return builtin::concatenate(args, context);
    }
};

TEST_F(ConcatenateFunctionTest, NoArguments_ReturnsEmptyString) {
    auto result = callConcatenate({});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(ConcatenateFunctionTest, SingleTextArgument_ReturnsSameText) {
    auto result = callConcatenate({Value("hello")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello", result.asText());
}

TEST_F(ConcatenateFunctionTest, MultipleTextArguments_JoinsText) {
    auto result = callConcatenate({Value("hello"), Value(" "), Value("world")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello world", result.asText());
}

TEST_F(ConcatenateFunctionTest, MixedTypes_ConvertsToText) {
    auto result = callConcatenate({Value("Value: "), Value(42.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Value: 42", result.asText());
}

TEST_F(ConcatenateFunctionTest, BooleanValues_ConvertsToText) {
    auto result = callConcatenate({Value("Result: "), Value(true)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Result: TRUE", result.asText());
}

TEST_F(ConcatenateFunctionTest, FalseBooleanValue_ConvertsToText) {
    auto result = callConcatenate({Value("Result: "), Value(false)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Result: FALSE", result.asText());
}

TEST_F(ConcatenateFunctionTest, EmptyValues_ConvertsToEmptyString) {
    auto result = callConcatenate({Value("Start"), Value::empty(), Value("End")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("StartEnd", result.asText());
}

TEST_F(ConcatenateFunctionTest, NumberWithDecimals_ConvertsCorrectly) {
    auto result = callConcatenate({Value("Pi is "), Value(3.14159)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Pi is 3.14159", result.asText());
}

TEST_F(ConcatenateFunctionTest, Zero_ConvertsToZeroString) {
    auto result = callConcatenate({Value("Count: "), Value(0.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Count: 0", result.asText());
}

TEST_F(ConcatenateFunctionTest, NegativeNumber_ConvertsCorrectly) {
    auto result = callConcatenate({Value("Temperature: "), Value(-5.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Temperature: -5", result.asText());
}

TEST_F(ConcatenateFunctionTest, ErrorValue_PropagatesError) {
    auto result =
            callConcatenate({Value("hello"), Value::error(ErrorType::REF_ERROR), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(ConcatenateFunctionTest, MultipleErrorValues_ReturnsFirstError) {
    auto result = callConcatenate(
            {Value::error(ErrorType::DIV_ZERO), Value::error(ErrorType::VALUE_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ConcatenateFunctionTest, LongString_HandlesCorrectly) {
    std::string longText = "This is a very long string that should be handled correctly";
    auto result = callConcatenate({Value("Prefix: "), Value(longText), Value(" :Suffix")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Prefix: " + longText + " :Suffix", result.asText());
}