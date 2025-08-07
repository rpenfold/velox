#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;

class CombinFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        engine = std::make_unique<FormulaEngine>();
    }

    std::unique_ptr<FormulaEngine> engine;
};

TEST_F(CombinFunctionTest, BasicCombination) {
    auto result = engine->evaluate("COMBIN(5, 2)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 10.0);
}

TEST_F(CombinFunctionTest, ChooseZero) {
    auto result = engine->evaluate("COMBIN(5, 0)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 1.0);
}

TEST_F(CombinFunctionTest, ChooseAll) {
    auto result = engine->evaluate("COMBIN(5, 5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 1.0);
}

TEST_F(CombinFunctionTest, Symmetry) {
    auto result1 = engine->evaluate("COMBIN(10, 3)");
    auto result2 = engine->evaluate("COMBIN(10, 7)");
    ASSERT_TRUE(result1.isSuccess());
    ASSERT_TRUE(result2.isSuccess());
    EXPECT_DOUBLE_EQ(result1.getValue().asNumber(), result2.getValue().asNumber());
}

TEST_F(CombinFunctionTest, KGreaterThanN) {
    auto result = engine->evaluate("COMBIN(3, 5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(CombinFunctionTest, NegativeNumbers) {
    auto result = engine->evaluate("COMBIN(-1, 2)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(CombinFunctionTest, NonIntegers) {
    auto result = engine->evaluate("COMBIN(5.5, 2)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(CombinFunctionTest, TooFewArguments) {
    auto result = engine->evaluate("COMBIN(5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(CombinFunctionTest, TooManyArguments) {
    auto result = engine->evaluate("COMBIN(5, 2, 3)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}