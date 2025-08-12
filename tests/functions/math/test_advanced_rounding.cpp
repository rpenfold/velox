#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class AdvancedRoundingTest : public ::testing::Test {
  protected:
    Context context;

    Value callMround(const std::vector<Value>& args) {
        return builtin::mround(args, context);
    }

    Value callRoundup(const std::vector<Value>& args) {
        return builtin::roundup(args, context);
    }

    Value callRounddown(const std::vector<Value>& args) {
        return builtin::rounddown(args, context);
    }
};

// MROUND Tests
TEST_F(AdvancedRoundingTest, Mround_TooFewArguments_ReturnsError) {
    auto result = callMround({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());

    result = callMround({Value(10.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(AdvancedRoundingTest, Mround_BasicRounding_WorksCorrectly) {
    auto result = callMround({Value(10.0), Value(3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(9.0, result.asNumber());  // Nearest multiple of 3 to 10 is 9

    result = callMround({Value(11.0), Value(3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(12.0, result.asNumber());  // Nearest multiple of 3 to 11 is 12
}

TEST_F(AdvancedRoundingTest, Mround_ZeroMultiple_ReturnsZero) {
    auto result = callMround({Value(10.0), Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(AdvancedRoundingTest, Mround_DifferentSigns_ReturnsError) {
    auto result = callMround({Value(10.0), Value(-3.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());

    result = callMround({Value(-10.0), Value(3.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(AdvancedRoundingTest, Mround_BothNegative_WorksCorrectly) {
    auto result = callMround({Value(-10.0), Value(-3.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-9.0, result.asNumber());
}

TEST_F(AdvancedRoundingTest, Mround_DecimalMultiple_WorksCorrectly) {
    auto result = callMround({Value(1.3), Value(0.2)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_NEAR(1.4, result.asNumber(), 1e-10);  // Nearest multiple of 0.2 to 1.3 is 1.4
}

// ROUNDUP Tests
TEST_F(AdvancedRoundingTest, Roundup_TooFewArguments_ReturnsError) {
    auto result = callRoundup({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());

    result = callRoundup({Value(3.2)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(AdvancedRoundingTest, Roundup_PositiveNumber_RoundsAwayFromZero) {
    auto result = callRoundup({Value(3.2), Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());

    result = callRoundup({Value(3.0), Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // Already integer
}

TEST_F(AdvancedRoundingTest, Roundup_NegativeNumber_RoundsAwayFromZero) {
    auto result = callRoundup({Value(-3.2), Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-4.0, result.asNumber());  // Away from zero means more negative
}

TEST_F(AdvancedRoundingTest, Roundup_DecimalPlaces_WorksCorrectly) {
    auto result = callRoundup({Value(3.14159), Value(2.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_NEAR(3.15, result.asNumber(), 1e-10);

    result = callRoundup({Value(3.14159), Value(4.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_NEAR(3.1416, result.asNumber(), 1e-10);
}

TEST_F(AdvancedRoundingTest, Roundup_NegativeDigits_WorksCorrectly) {
    auto result = callRoundup({Value(31.9), Value(-1.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(40.0, result.asNumber());  // Round to tens place
}

TEST_F(AdvancedRoundingTest, Roundup_Zero_ReturnsZero) {
    auto result = callRoundup({Value(0.0), Value(2.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

// ROUNDDOWN Tests
TEST_F(AdvancedRoundingTest, Rounddown_TooFewArguments_ReturnsError) {
    auto result = callRounddown({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());

    result = callRounddown({Value(3.9)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(AdvancedRoundingTest, Rounddown_PositiveNumber_RoundsTowardZero) {
    auto result = callRounddown({Value(3.9), Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());

    result = callRounddown({Value(3.0), Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // Already integer
}

TEST_F(AdvancedRoundingTest, Rounddown_NegativeNumber_RoundsTowardZero) {
    auto result = callRounddown({Value(-3.9), Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());  // Toward zero means less negative
}

TEST_F(AdvancedRoundingTest, Rounddown_DecimalPlaces_WorksCorrectly) {
    auto result = callRounddown({Value(3.14159), Value(2.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_NEAR(3.14, result.asNumber(), 1e-10);

    result = callRounddown({Value(3.14159), Value(4.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_NEAR(3.1415, result.asNumber(), 1e-10);
}

TEST_F(AdvancedRoundingTest, Rounddown_NegativeDigits_WorksCorrectly) {
    auto result = callRounddown({Value(38.9), Value(-1.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(30.0, result.asNumber());  // Round to tens place
}

TEST_F(AdvancedRoundingTest, Rounddown_Zero_ReturnsZero) {
    auto result = callRounddown({Value(0.0), Value(2.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

// Error Propagation Tests for all functions
TEST_F(AdvancedRoundingTest, AllFunctions_ErrorInput_PropagatesError) {
    auto result = callMround({Value::error(ErrorType::VALUE_ERROR), Value(3.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());

    result = callRoundup({Value::error(ErrorType::DIV_ZERO), Value(2.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());

    result = callRounddown({Value(3.2), Value::error(ErrorType::NUM_ERROR)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}
