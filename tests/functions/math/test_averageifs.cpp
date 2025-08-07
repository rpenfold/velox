#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;

class AverageifsFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        engine = std::make_unique<FormulaEngine>();
    }

    std::unique_ptr<FormulaEngine> engine;
};

TEST_F(AverageifsFunctionTest, SingleCriteria) {
    auto result = engine->evaluate("AVERAGEIFS(10, 5, 5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 10.0);
}

TEST_F(AverageifsFunctionTest, MultipleCriteriaMatch) {
    auto result = engine->evaluate("AVERAGEIFS(20, 5, 5, 8, 8)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 20.0);
}

TEST_F(AverageifsFunctionTest, MultipleCriteriaNoMatch) {
    auto result = engine->evaluate("AVERAGEIFS(20, 5, 5, 8, 7)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
    EXPECT_EQ(result.getValue().asError(), ErrorType::DIV_ZERO);
}

TEST_F(AverageifsFunctionTest, GreaterThanCriteria) {
    auto result = engine->evaluate("AVERAGEIFS(15, 10, \">5\", 3, \"<5\")");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 15.0);
}

TEST_F(AverageifsFunctionTest, TooFewArguments) {
    auto result = engine->evaluate("AVERAGEIFS(10, 5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(AverageifsFunctionTest, EvenNumberOfArguments) {
    auto result = engine->evaluate("AVERAGEIFS(10, 5, 5, 8)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}