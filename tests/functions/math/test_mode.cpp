#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class ModeFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callMode(const std::vector<Value>& args) {
        return builtin::mode(args, context);
    }
};

TEST_F(ModeFunctionTest, EmptyArguments_ReturnsError) {
    auto result = callMode({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ModeFunctionTest, SingleNumber_ReturnsSameNumber) {
    auto result = callMode({Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(ModeFunctionTest, TwoDifferentNumbers_ReturnsFirstNumber) {
    auto result = callMode({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(ModeFunctionTest, TwoSameNumbers_ReturnsThatNumber) {
    auto result = callMode({Value(2.0), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(ModeFunctionTest, ThreeNumbersWithMode_ReturnsMode) {
    auto result = callMode({Value(1.0), Value(2.0), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(ModeFunctionTest, AllDifferentNumbers_ReturnsFirstNumber) {
    auto result = callMode({Value(1.0), Value(2.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(ModeFunctionTest, MultipleModes_ReturnsFirstMode) {
    auto result = callMode({Value(1.0), Value(1.0), Value(2.0), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    // Both 1 and 2 appear twice, should return the first one found (1)
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(ModeFunctionTest, NegativeNumbers_ReturnsCorrectMode) {
    auto result = callMode({Value(-10.0), Value(-5.0), Value(-5.0), Value(-1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-5.0, result.asNumber());
}

TEST_F(ModeFunctionTest, MixedPositiveNegative_ReturnsCorrectMode) {
    auto result = callMode({Value(10.0), Value(-5.0), Value(10.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(10.0, result.asNumber());
}

TEST_F(ModeFunctionTest, WithBooleans_ConvertsBooleans) {
    auto result = callMode({Value(1.0), Value(true), Value(true), Value(4.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());  // true converts to 1, so 1 appears twice
}

TEST_F(ModeFunctionTest, WithTextValues_IgnoresText) {
    auto result = callMode({Value(1.0), Value("hello"), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());  // 1 appears twice
}

TEST_F(ModeFunctionTest, WithEmptyValues_IgnoresEmpty) {
    auto result = callMode({Value(2.0), Value::empty(), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // 2 appears twice
}

TEST_F(ModeFunctionTest, OnlyTextValues_ReturnsError) {
    auto result = callMode({Value("hello"), Value("world")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ModeFunctionTest, OnlyEmptyValues_ReturnsError) {
    auto result = callMode({Value::empty(), Value::empty()});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(ModeFunctionTest, WithErrorValue_PropagatesError) {
    auto result = callMode({Value(1.0), Value::error(ErrorType::REF_ERROR), Value(5.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::REF_ERROR, result.asError());
}

TEST_F(ModeFunctionTest, DecimalNumbers_ReturnsCorrectMode) {
    auto result = callMode({Value(1.5), Value(2.25), Value(1.5), Value(0.25)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.5, result.asNumber());
}

TEST_F(ModeFunctionTest, LargeNumbers_HandlesCorrectly) {
    auto result = callMode({Value(1000000.0), Value(2000000.0), Value(1000000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1000000.0, result.asNumber());
}

TEST_F(ModeFunctionTest, ZeroValues_HandlesCorrectly) {
    auto result = callMode({Value(0.0), Value(0.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ModeFunctionTest, ComplexModeScenario_ReturnsCorrectMode) {
    auto result = callMode({Value(1.0), Value(2.0), Value(2.0), Value(3.0), Value(3.0), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // 3 appears most frequently
}

TEST_F(ModeFunctionTest, AllSameNumbers_ReturnsThatNumber) {
    auto result = callMode({Value(5.0), Value(5.0), Value(5.0), Value(5.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}