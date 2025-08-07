#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;

class SumifFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        engine = std::make_unique<FormulaEngine>();
    }

    std::unique_ptr<FormulaEngine> engine;
};

TEST_F(SumifFunctionTest, NumericCriteria) {
    auto result = engine->evaluate("SUMIF(5, 5, 10)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 10.0);
}

TEST_F(SumifFunctionTest, NumericCriteriaNoMatch) {
    auto result = engine->evaluate("SUMIF(5, 3, 10)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 0.0);
}

TEST_F(SumifFunctionTest, GreaterThanCriteria) {
    auto result = engine->evaluate("SUMIF(10, \">5\", 20)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 20.0);
}

TEST_F(SumifFunctionTest, LessThanCriteria) {
    auto result = engine->evaluate("SUMIF(3, \"<5\", 15)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 15.0);
}

TEST_F(SumifFunctionTest, EqualCriteria) {
    auto result = engine->evaluate("SUMIF(7, \"=7\", 14)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 14.0);
}

TEST_F(SumifFunctionTest, NotEqualCriteria) {
    auto result = engine->evaluate("SUMIF(7, \"<>5\", 14)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 14.0);
}

TEST_F(SumifFunctionTest, TwoArgumentsOnly) {
    auto result = engine->evaluate("SUMIF(8, 8)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 8.0);
}

TEST_F(SumifFunctionTest, TooFewArguments) {
    auto result = engine->evaluate("SUMIF(5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(SumifFunctionTest, TooManyArguments) {
    auto result = engine->evaluate("SUMIF(5, 5, 10, 15)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}