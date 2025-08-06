#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class ModFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callMod(const std::vector<Value>& args) {
        return builtin::mod(args, context);
    }
};

TEST_F(ModFunctionTest, NoArguments_ReturnsError) {
    auto result = callMod({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ModFunctionTest, OneArgument_ReturnsError) {
    auto result = callMod({Value(10.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ModFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callMod({Value(10.0), Value(3.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ModFunctionTest, PositiveNumbers_ReturnsRemainder) {
    auto result = callMod({Value(10.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(ModFunctionTest, ExactDivision_ReturnsZero) {
    auto result = callMod({Value(15.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ModFunctionTest, DivisionByZero_ReturnsError) {
    auto result = callMod({Value(10.0), Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ModFunctionTest, NegativeNumerator_PositiveDivisor_ReturnsPositiveRemainder) {
    auto result = callMod({Value(-10.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // Excel behavior: sign follows divisor
}

TEST_F(ModFunctionTest, PositiveNumerator_NegativeDivisor_ReturnsNegativeRemainder) {
    auto result = callMod({Value(10.0), Value(-3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-2.0, result.asNumber());  // Excel behavior: sign follows divisor
}

TEST_F(ModFunctionTest, BothNegative_ReturnsNegativeRemainder) {
    auto result = callMod({Value(-10.0), Value(-3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());
}

TEST_F(ModFunctionTest, DecimalNumbers_ReturnsCorrectRemainder) {
    auto result = callMod({Value(10.5), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1.5, result.asNumber(), 1e-10);
}

TEST_F(ModFunctionTest, DecimalDivisor_ReturnsCorrectRemainder) {
    auto result = callMod({Value(10.0), Value(3.5)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(3.0, result.asNumber(), 1e-10);
}

TEST_F(ModFunctionTest, SmallerNumerator_ReturnsNumerator) {
    auto result = callMod({Value(2.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(ModFunctionTest, ZeroNumerator_ReturnsZero) {
    auto result = callMod({Value(0.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ModFunctionTest, BooleanInputs_ConvertsToNumbers) {
    auto result = callMod({Value(true), Value(false)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());  // 1 MOD 0 = error
}

TEST_F(ModFunctionTest, BooleanInputs_ValidCase_ConvertsToNumbers) {
    auto result = callMod({Value(false), Value(true)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());  // 0 MOD 1 = 0
}

TEST_F(ModFunctionTest, NumericTextInputs_ConvertsToNumbers) {
    auto result = callMod({Value("10"), Value("3")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(ModFunctionTest, NonNumericTextNumerator_ReturnsError) {
    auto result = callMod({Value("hello"), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ModFunctionTest, NonNumericTextDivisor_ReturnsError) {
    auto result = callMod({Value(10.0), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ModFunctionTest, ErrorInNumerator_PropagatesError) {
    auto result = callMod({Value::error(ErrorType::REF_ERROR), Value(3.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(ModFunctionTest, ErrorInDivisor_PropagatesError) {
    auto result = callMod({Value(10.0), Value::error(ErrorType::VALUE_ERROR)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ModFunctionTest, LargeNumbers_HandlesCorrectly) {
    auto result = callMod({Value(1000000.0), Value(7.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());  // 1000000 % 7 = 1
}