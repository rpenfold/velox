#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;

class SumifsFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        engine = std::make_unique<FormulaEngine>();
    }

    std::unique_ptr<FormulaEngine> engine;
};

TEST_F(SumifsFunctionTest, SingleCriteria) {
    auto result = engine->evaluate("SUMIFS(10, 5, 5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 10.0);
}

TEST_F(SumifsFunctionTest, MultipleCriteriaMatch) {
    auto result = engine->evaluate("SUMIFS(20, 5, 5, 8, 8)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 20.0);
}

TEST_F(SumifsFunctionTest, MultipleCriteriaNoMatch) {
    auto result = engine->evaluate("SUMIFS(20, 5, 5, 8, 7)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 0.0);
}

TEST_F(SumifsFunctionTest, GreaterThanCriteria) {
    auto result = engine->evaluate("SUMIFS(15, 10, \">5\", 3, \"<5\")");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 15.0);
}

TEST_F(SumifsFunctionTest, TooFewArguments) {
    auto result = engine->evaluate("SUMIFS(10, 5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(SumifsFunctionTest, EvenNumberOfArguments) {
    auto result = engine->evaluate("SUMIFS(10, 5, 5, 8)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}