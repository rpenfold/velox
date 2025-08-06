#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class VarFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callVar(const std::vector<Value>& args) {
        return builtin::var(args, context);
    }
};

TEST_F(VarFunctionTest, EmptyArguments_ReturnsError) {
    auto result = callVar({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(VarFunctionTest, SingleNumber_ReturnsError) {
    auto result = callVar({Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(VarFunctionTest, TwoNumbers_ReturnsCorrectVariance) {
    auto result = callVar({Value(1.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    // Expected: ((1-2)^2 + (3-2)^2) / (2-1) = 2
    EXPECT_NEAR(2.0, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, ThreeNumbers_ReturnsCorrectVariance) {
    auto result = callVar({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 2, Variance = ((1-2)^2 + (2-2)^2 + (3-2)^2) / (3-1) = 1
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, FourNumbers_ReturnsCorrectVariance) {
    auto result = callVar({Value(1.0), Value(2.0), Value(3.0), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 2.5, Variance = ((1-2.5)^2 + (2-2.5)^2 + (3-2.5)^2 + (4-2.5)^2) / (4-1) = 1.666...
    EXPECT_NEAR(1.6666666666666667, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, NegativeNumbers_ReturnsCorrectVariance) {
    auto result = callVar({Value(-10.0), Value(-5.0), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = -5.333..., Variance = 20.333...
    EXPECT_NEAR(20.333333333333336, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, MixedPositiveNegative_ReturnsCorrectVariance) {
    auto result = callVar({Value(10.0), Value(-5.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 2, Variance = 57
    EXPECT_NEAR(57.0, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, WithBooleans_ConvertsBooleans) {
    auto result = callVar({Value(1.0), Value(true), Value(false), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    // Values: [1, 1, 0, 4], Mean = 1.5, Variance = 3
    EXPECT_NEAR(3.0, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, WithTextValues_IgnoresText) {
    auto result = callVar({Value(1.0), Value("hello"), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    // Values: [1, 5], Mean = 3, Variance = 8
    EXPECT_NEAR(8.0, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, WithEmptyValues_IgnoresEmpty) {
    auto result = callVar({Value(2.0), Value::empty(), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    // Values: [2, 4], Mean = 3, Variance = 2
    EXPECT_NEAR(2.0, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, OnlyTextValues_ReturnsError) {
    auto result = callVar({Value("hello"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(VarFunctionTest, OnlyEmptyValues_ReturnsError) {
    auto result = callVar({Value::empty(), Value::empty()});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(VarFunctionTest, WithErrorValue_PropagatesError) {
    auto result = callVar({Value(1.0), Value::error(ErrorType::REF_ERROR), Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(VarFunctionTest, DecimalNumbers_ReturnsCorrectVariance) {
    auto result = callVar({Value(1.5), Value(2.25), Value(0.25)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 1.333..., Variance = 1.020...
    EXPECT_NEAR(1.0208333333333333, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, LargeNumbers_HandlesCorrectly) {
    auto result = callVar({Value(1000000.0), Value(2000000.0), Value(3000000.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 2000000, Variance = 1000000000000
    EXPECT_NEAR(1000000000000.0, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, ZeroValues_ReturnsZero) {
    auto result = callVar({Value(0.0), Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, AllSameNumbers_ReturnsZero) {
    auto result = callVar({Value(5.0), Value(5.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, ComplexScenario_ReturnsCorrectVariance) {
    auto result = callVar({Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 3, Variance = 2.5
    EXPECT_NEAR(2.5, result.asNumber(), 1e-10);
}

TEST_F(VarFunctionTest, ManyNumbers_ReturnsCorrectVariance) {
    auto result = callVar(
            {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0), Value(6.0), Value(7.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 4, Variance = 4.666...
    EXPECT_NEAR(4.666666666666667, result.asNumber(), 1e-10);
}