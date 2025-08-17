#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;

class AverageifFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        engine = std::make_unique<FormulaEngine>();
    }

    std::unique_ptr<FormulaEngine> engine;
};

TEST_F(AverageifFunctionTest, NumericCriteria) {
    auto result = engine->evaluate("AVERAGEIF(5, 5, 10)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 10.0);
}

TEST_F(AverageifFunctionTest, NumericCriteriaNoMatch) {
    auto result = engine->evaluate("AVERAGEIF(5, 3, 10)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
    EXPECT_EQ(result.getValue().asError(), ErrorType::DIV_ZERO);
}

TEST_F(AverageifFunctionTest, GreaterThanCriteria) {
    auto result = engine->evaluate("AVERAGEIF(10, \">5\", 20)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 20.0);
}

TEST_F(AverageifFunctionTest, LessThanCriteria) {
    auto result = engine->evaluate("AVERAGEIF(3, \"<5\", 15)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 15.0);
}

TEST_F(AverageifFunctionTest, EqualCriteria) {
    auto result = engine->evaluate("AVERAGEIF(7, \"=7\", 14)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 14.0);
}

TEST_F(AverageifFunctionTest, TwoArgumentsOnly) {
    auto result = engine->evaluate("AVERAGEIF(8, 8)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 8.0);
}

TEST_F(AverageifFunctionTest, TooFewArguments) {
    auto result = engine->evaluate("AVERAGEIF(5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(AverageifFunctionTest, TooManyArguments) {
    auto result = engine->evaluate("AVERAGEIF(5, 5, 10, 15)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}