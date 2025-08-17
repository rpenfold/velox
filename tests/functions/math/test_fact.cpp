#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;

class FactFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        engine = std::make_unique<FormulaEngine>();
    }

    std::unique_ptr<FormulaEngine> engine;
};

TEST_F(FactFunctionTest, ZeroFactorial) {
    auto result = engine->evaluate("FACT(0)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 1.0);
}

TEST_F(FactFunctionTest, SmallPositiveNumbers) {
    auto result = engine->evaluate("FACT(5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 120.0);
}

TEST_F(FactFunctionTest, LargerNumber) {
    auto result = engine->evaluate("FACT(10)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 3628800.0);
}

TEST_F(FactFunctionTest, NegativeNumber) {
    auto result = engine->evaluate("FACT(-1)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(FactFunctionTest, NonInteger) {
    auto result = engine->evaluate("FACT(5.5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(FactFunctionTest, TooLargeNumber) {
    auto result = engine->evaluate("FACT(171)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(FactFunctionTest, NoArguments) {
    auto result = engine->evaluate("FACT()");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(FactFunctionTest, TooManyArguments) {
    auto result = engine->evaluate("FACT(5, 3)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}