#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class SubstituteFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callSubstitute(const std::vector<Value>& args) {
        return builtin::substitute(args, context);
    }
};

TEST_F(SubstituteFunctionTest, NoArguments_ReturnsError) {
    auto result = callSubstitute({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SubstituteFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callSubstitute({Value("hello"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SubstituteFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callSubstitute(
            {Value("hello"), Value("world"), Value("earth"), Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SubstituteFunctionTest, NonNumericInstanceNum_ReturnsError) {
    auto result = callSubstitute({Value("hello"), Value("world"), Value("earth"), Value("abc")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SubstituteFunctionTest, InstanceNumLessThanOne_ReturnsError) {
    auto result = callSubstitute({Value("hello"), Value("world"), Value("earth"), Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(SubstituteFunctionTest, BasicSubstitute_ReplacesAllOccurrences) {
    auto result = callSubstitute({Value("hello world hello"), Value("hello"), Value("goodbye")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("goodbye world goodbye", result.asText());
}

TEST_F(SubstituteFunctionTest, SubstituteSpecificInstance_ReplacesOnlySpecified) {
    auto result = callSubstitute(
            {Value("hello world hello"), Value("hello"), Value("goodbye"), Value(1.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("goodbye world hello", result.asText());
}

TEST_F(SubstituteFunctionTest, SubstituteSecondInstance_ReplacesSecondOccurrence) {
    auto result = callSubstitute(
            {Value("hello world hello"), Value("hello"), Value("goodbye"), Value(2.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello world goodbye", result.asText());
}

TEST_F(SubstituteFunctionTest, InstanceNumBeyondOccurrences_ReturnsOriginal) {
    auto result =
            callSubstitute({Value("hello world"), Value("hello"), Value("goodbye"), Value(3.0)});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello world", result.asText());
}

TEST_F(SubstituteFunctionTest, EmptyOldText_ReturnsOriginal) {
    auto result = callSubstitute({Value("hello world"), Value(""), Value("goodbye")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello world", result.asText());
}

TEST_F(SubstituteFunctionTest, EmptyNewText_RemovesOldText) {
    auto result = callSubstitute({Value("hello world"), Value("hello"), Value("")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ(" world", result.asText());
}

TEST_F(SubstituteFunctionTest, SubstituteNotPresent_ReturnsOriginal) {
    auto result = callSubstitute({Value("hello world"), Value("xyz"), Value("goodbye")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello world", result.asText());
}

TEST_F(SubstituteFunctionTest, SubstituteCaseSensitive_RespectsCase) {
    auto result = callSubstitute({Value("Hello world"), Value("hello"), Value("goodbye")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("Hello world", result.asText());
}

TEST_F(SubstituteFunctionTest, SubstituteWithSpaces_HandlesSpaces) {
    auto result = callSubstitute({Value("hello world"), Value(" "), Value("_")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello_world", result.asText());
}

TEST_F(SubstituteFunctionTest, SubstituteWithSpecialCharacters_HandlesSpecialChars) {
    auto result = callSubstitute({Value("hello!world"), Value("!"), Value("@")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello@world", result.asText());
}

TEST_F(SubstituteFunctionTest, SubstituteWithNumbers_HandlesNumbers) {
    auto result = callSubstitute({Value("hello123world"), Value("123"), Value("456")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("hello456world", result.asText());
}

TEST_F(SubstituteFunctionTest, SubstituteMultipleOccurrences_ReplacesAll) {
    auto result = callSubstitute({Value("hello hello hello"), Value("hello"), Value("goodbye")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("goodbye goodbye goodbye", result.asText());
}

TEST_F(SubstituteFunctionTest, SubstituteOverlappingText_HandlesOverlap) {
    auto result = callSubstitute({Value("hellohello"), Value("hello"), Value("goodbye")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("goodbyegoodbye", result.asText());
}

TEST_F(SubstituteFunctionTest, SubstituteEmptyString_HandlesEmpty) {
    auto result = callSubstitute({Value(""), Value("hello"), Value("goodbye")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("", result.asText());
}

TEST_F(SubstituteFunctionTest, NumberInput_ConvertsToTextFirst) {
    auto result = callSubstitute({Value(123.45), Value("123"), Value("456")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("456.45", result.asText());
}

TEST_F(SubstituteFunctionTest, BooleanInput_ConvertsToTextFirst) {
    auto result = callSubstitute({Value(true), Value("TRUE"), Value("FALSE")});

    EXPECT_TRUE(result.isText());
    EXPECT_EQ("FALSE", result.asText());
}

TEST_F(SubstituteFunctionTest, ErrorInput_PropagatesError) {
    auto result =
            callSubstitute({Value::error(ErrorType::DIV_ZERO), Value("hello"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(SubstituteFunctionTest, ErrorInSecondArgument_PropagatesError) {
    auto result =
            callSubstitute({Value("hello"), Value::error(ErrorType::DIV_ZERO), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(SubstituteFunctionTest, ErrorInThirdArgument_PropagatesError) {
    auto result =
            callSubstitute({Value("hello"), Value("world"), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(SubstituteFunctionTest, ErrorInFourthArgument_PropagatesError) {
    auto result = callSubstitute(
            {Value("hello"), Value("world"), Value("earth"), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}