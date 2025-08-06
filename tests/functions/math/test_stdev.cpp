#include <gtest/gtest.h>
#include <xl-formula/functions.h>
#include <cmath>

using namespace xl_formula;
using namespace xl_formula::functions;

class StdevFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callStdev(const std::vector<Value>& args) {
        return builtin::stdev(args, context);
    }
};

TEST_F(StdevFunctionTest, EmptyArguments_ReturnsError) {
    auto result = callStdev({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(StdevFunctionTest, SingleNumber_ReturnsError) {
    auto result = callStdev({Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(StdevFunctionTest, TwoNumbers_ReturnsCorrectStdev) {
    auto result = callStdev({Value(1.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    // Expected: sqrt(((1-2)^2 + (3-2)^2) / (2-1)) = sqrt(2) ≈ 1.414
    EXPECT_NEAR(1.4142135623730951, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, ThreeNumbers_ReturnsCorrectStdev) {
    auto result = callStdev({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 2, Variance = ((1-2)^2 + (2-2)^2 + (3-2)^2) / (3-1) = 1
    // Stdev = sqrt(1) = 1
    EXPECT_NEAR(1.0, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, FourNumbers_ReturnsCorrectStdev) {
    auto result = callStdev({Value(1.0), Value(2.0), Value(3.0), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 2.5, Variance = ((1-2.5)^2 + (2-2.5)^2 + (3-2.5)^2 + (4-2.5)^2) / (4-1) = 1.666...
    // Stdev = sqrt(1.666...) ≈ 1.29099
    EXPECT_NEAR(1.2909944487358056, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, NegativeNumbers_ReturnsCorrectStdev) {
    auto result = callStdev({Value(-10.0), Value(-5.0), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = -5.333..., Variance = 20.333..., Stdev ≈ 4.509
    EXPECT_NEAR(4.5092497528228943, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, MixedPositiveNegative_ReturnsCorrectStdev) {
    auto result = callStdev({Value(10.0), Value(-5.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 2, Variance = 57, Stdev ≈ 7.55
    EXPECT_NEAR(7.5498344352707498, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, WithBooleans_ConvertsBooleans) {
    auto result = callStdev({Value(1.0), Value(true), Value(false), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    // Values: [1, 1, 0, 4], Mean = 1.5, Variance = 3, Stdev ≈ 1.732
    EXPECT_NEAR(1.7320508075688772, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, WithTextValues_IgnoresText) {
    auto result = callStdev({Value(1.0), Value("hello"), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    // Values: [1, 5], Mean = 3, Variance = 8, Stdev = sqrt(8) ≈ 2.828
    EXPECT_NEAR(2.8284271247461903, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, WithEmptyValues_IgnoresEmpty) {
    auto result = callStdev({Value(2.0), Value::empty(), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    // Values: [2, 4], Mean = 3, Variance = 2, Stdev = sqrt(2) ≈ 1.414
    EXPECT_NEAR(1.4142135623730951, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, OnlyTextValues_ReturnsError) {
    auto result = callStdev({Value("hello"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(StdevFunctionTest, OnlyEmptyValues_ReturnsError) {
    auto result = callStdev({Value::empty(), Value::empty()});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(StdevFunctionTest, WithErrorValue_PropagatesError) {
    auto result = callStdev({Value(1.0), Value::error(ErrorType::REF_ERROR), Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(StdevFunctionTest, DecimalNumbers_ReturnsCorrectStdev) {
    auto result = callStdev({Value(1.5), Value(2.25), Value(0.25)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 1.333..., Variance = 1.020..., Stdev ≈ 1.010
    EXPECT_NEAR(1.0103629710818451, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, LargeNumbers_HandlesCorrectly) {
    auto result = callStdev({Value(1000000.0), Value(2000000.0), Value(3000000.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 2000000, Variance = 1000000000000, Stdev = 1000000
    EXPECT_NEAR(1000000.0, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, ZeroValues_ReturnsZero) {
    auto result = callStdev({Value(0.0), Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, AllSameNumbers_ReturnsZero) {
    auto result = callStdev({Value(5.0), Value(5.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0, result.asNumber(), 1e-10);
}

TEST_F(StdevFunctionTest, ComplexScenario_ReturnsCorrectStdev) {
    auto result = callStdev({Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    // Mean = 3, Variance = 2.5, Stdev = sqrt(2.5) ≈ 1.581
    EXPECT_NEAR(1.5811388300841898, result.asNumber(), 1e-10);
}