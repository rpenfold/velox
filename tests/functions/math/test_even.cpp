#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class EvenFunctionTest : public ::testing::Test {
protected:
    Context context;
    
    Value callEven(const std::vector<Value>& args) {
        return builtin::even_function(args, context);
    }
};

TEST_F(EvenFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callEven({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(EvenFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callEven({Value(1.0), Value(2.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(EvenFunctionTest, Zero_ReturnsZero) {
    auto result = callEven({Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(EvenFunctionTest, PositiveOddInteger_RoundsUp) {
    auto result = callEven({Value(1.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
    
    result = callEven({Value(3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(EvenFunctionTest, PositiveEvenInteger_StaysSame) {
    auto result = callEven({Value(2.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
    
    result = callEven({Value(4.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(EvenFunctionTest, PositiveDecimal_RoundsUpToNextEven) {
    auto result = callEven({Value(1.5)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
    
    result = callEven({Value(2.3)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
    
    result = callEven({Value(3.7)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(EvenFunctionTest, NegativeOddInteger_RoundsAwayFromZero) {
    auto result = callEven({Value(-1.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-2.0, result.asNumber());
    
    result = callEven({Value(-3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-4.0, result.asNumber());
}

TEST_F(EvenFunctionTest, NegativeEvenInteger_StaysSame) {
    auto result = callEven({Value(-2.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-2.0, result.asNumber());
    
    result = callEven({Value(-4.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-4.0, result.asNumber());
}

TEST_F(EvenFunctionTest, NegativeDecimal_RoundsAwayFromZero) {
    auto result = callEven({Value(-1.5)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-2.0, result.asNumber());
    
    result = callEven({Value(-2.3)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-4.0, result.asNumber());
    
    result = callEven({Value(-3.7)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-4.0, result.asNumber());
}

TEST_F(EvenFunctionTest, LargeNumbers_WorksCorrectly) {
    auto result = callEven({Value(999.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1000.0, result.asNumber());
    
    result = callEven({Value(-999.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1000.0, result.asNumber());
}

TEST_F(EvenFunctionTest, ErrorInput_PropagatesError) {
    auto result = callEven({Value::error(ErrorType::VALUE_ERROR)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(EvenFunctionTest, TextInput_ConvertsIfNumeric) {
    auto result = callEven({Value("3")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(EvenFunctionTest, BooleanInput_ConvertsCorrectly) {
    auto result = callEven({Value(true)});  // TRUE = 1
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
    
    result = callEven({Value(false)});  // FALSE = 0
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}
