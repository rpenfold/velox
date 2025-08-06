#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class XorFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callXor(const std::vector<Value>& args) {
        return builtin::xor_function(args, context);
    }
};

TEST_F(XorFunctionTest, NoArguments_ReturnsError) {
    auto result = callXor({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(XorFunctionTest, SingleTrueArgument_ReturnsTrue) {
    auto result = callXor({Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, SingleFalseArgument_ReturnsFalse) {
    auto result = callXor({Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(XorFunctionTest, TwoTrueArguments_ReturnsFalse) {
    auto result = callXor({Value(true), Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(XorFunctionTest, TwoFalseArguments_ReturnsFalse) {
    auto result = callXor({Value(false), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(XorFunctionTest, OneTrueOneFalse_ReturnsTrue) {
    auto result = callXor({Value(true), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, ThreeTrueArguments_ReturnsTrue) {
    auto result = callXor({Value(true), Value(true), Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, ThreeFalseArguments_ReturnsFalse) {
    auto result = callXor({Value(false), Value(false), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(XorFunctionTest, TwoTrueOneFalse_ReturnsFalse) {
    auto result = callXor({Value(true), Value(true), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(XorFunctionTest, OneTrueTwoFalse_ReturnsTrue) {
    auto result = callXor({Value(true), Value(false), Value(false)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, NonZeroNumbers_EvaluateToTrue) {
    auto result = callXor({Value(1.0), Value(0.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, AllNonZeroNumbers_ReturnsTrue) {
    auto result = callXor({Value(1.0), Value(5.0), Value(3.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, AllZeroNumbers_ReturnsFalse) {
    auto result = callXor({Value(0.0), Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(XorFunctionTest, NonEmptyText_EvaluatesToTrue) {
    auto result = callXor({Value("hello"), Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, AllNonEmptyText_ReturnsTrue) {
    auto result = callXor({Value("hello"), Value("world"), Value("test")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, AllEmptyText_ReturnsFalse) {
    auto result = callXor({Value(""), Value(""), Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(XorFunctionTest, MixedTypes_HandlesCorrectly) {
    auto result = callXor({Value(true), Value(1.0), Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(XorFunctionTest, EmptyValues_EvaluateToFalse) {
    auto result = callXor({Value::empty(), Value::empty(), Value::empty()});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(XorFunctionTest, ErrorInArguments_PropagatesError) {
    auto result = callXor({Value(true), Value::error(ErrorType::VALUE_ERROR), Value(false)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(XorFunctionTest, BooleanConversion_WorksCorrectly) {
    // Test that numbers convert to boolean correctly
    auto result = callXor({Value(1.0), Value(0.0)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, TextConversion_WorksCorrectly) {
    // Test that text converts to boolean correctly
    auto result = callXor({Value("hello"), Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, WhitespaceText_EvaluatesToTrue) {
    auto result = callXor({Value(" "), Value("")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(XorFunctionTest, EvenNumberOfTrue_ReturnsFalse) {
    auto result = callXor({Value(true), Value(true), Value(true), Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(XorFunctionTest, OddNumberOfTrue_ReturnsTrue) {
    auto result = callXor({Value(true), Value(true), Value(true)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}