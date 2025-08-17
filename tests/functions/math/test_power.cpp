#include <gtest/gtest.h>
#include <velox/formulas/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class PowerFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callPower(const std::vector<Value>& args) {
        return builtin::power(args, context);
    }
};

TEST_F(PowerFunctionTest, NoArguments_ReturnsError) {
    auto result = callPower({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(PowerFunctionTest, OneArgument_ReturnsError) {
    auto result = callPower({Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(PowerFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callPower({Value(2.0), Value(3.0), Value(4.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(PowerFunctionTest, PositiveBasePositiveExponent_ReturnsCorrectPower) {
    auto result = callPower({Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(8.0, result.asNumber());
}

TEST_F(PowerFunctionTest, BaseToZeroPower_ReturnsOne) {
    auto result = callPower({Value(5.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(PowerFunctionTest, BaseToFirstPower_ReturnsBase) {
    auto result = callPower({Value(7.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(PowerFunctionTest, OneToAnyPower_ReturnsOne) {
    auto result = callPower({Value(1.0), Value(100.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(PowerFunctionTest, ZeroToPositivePower_ReturnsZero) {
    auto result = callPower({Value(0.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(PowerFunctionTest, ZeroToNegativePower_ReturnsError) {
    auto result = callPower({Value(0.0), Value(-2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(PowerFunctionTest, NegativeBaseIntegerExponent_ReturnsCorrectPower) {
    auto result = callPower({Value(-2.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-8.0, result.asNumber());
}

TEST_F(PowerFunctionTest, NegativeBaseEvenExponent_ReturnsPositivePower) {
    auto result = callPower({Value(-3.0), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(9.0, result.asNumber());
}

TEST_F(PowerFunctionTest, NegativeBaseNonIntegerExponent_ReturnsError) {
    auto result = callPower({Value(-2.0), Value(2.5)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(PowerFunctionTest, DecimalBaseAndExponent_ReturnsCorrectPower) {
    auto result = callPower({Value(2.5), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.25, result.asNumber());
}

TEST_F(PowerFunctionTest, NegativeExponent_ReturnsReciprocal) {
    auto result = callPower({Value(2.0), Value(-2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.25, result.asNumber());
}

TEST_F(PowerFunctionTest, FractionalExponent_ReturnsRoot) {
    auto result = callPower({Value(8.0), Value(1.0 / 3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2.0, result.asNumber(), 1e-10);
}

TEST_F(PowerFunctionTest, BooleanInputs_ConvertsToNumbers) {
    auto result = callPower({Value(true), Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());  // 1^0 = 1
}

TEST_F(PowerFunctionTest, NumericTextInputs_ConvertsToNumbers) {
    auto result = callPower({Value("2"), Value("3")});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(8.0, result.asNumber());
}

TEST_F(PowerFunctionTest, NonNumericTextBase_ReturnsError) {
    auto result = callPower({Value("hello"), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(PowerFunctionTest, NonNumericTextExponent_ReturnsError) {
    auto result = callPower({Value(2.0), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(PowerFunctionTest, ErrorInBase_PropagatesError) {
    auto result = callPower({Value::error(ErrorType::REF_ERROR), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(PowerFunctionTest, ErrorInExponent_PropagatesError) {
    auto result = callPower({Value(2.0), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}