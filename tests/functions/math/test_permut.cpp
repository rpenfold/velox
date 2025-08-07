#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;

class PermutFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        engine = std::make_unique<FormulaEngine>();
    }

    std::unique_ptr<FormulaEngine> engine;
};

TEST_F(PermutFunctionTest, BasicPermutation) {
    auto result = engine->evaluate("PERMUT(5, 2)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 20.0);
}

TEST_F(PermutFunctionTest, PermutationOfZero) {
    auto result = engine->evaluate("PERMUT(5, 0)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 1.0);
}

TEST_F(PermutFunctionTest, PermutationOfAll) {
    auto result = engine->evaluate("PERMUT(4, 4)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 24.0);
}

TEST_F(PermutFunctionTest, LargerExample) {
    auto result = engine->evaluate("PERMUT(10, 3)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 720.0);
}

TEST_F(PermutFunctionTest, KGreaterThanN) {
    auto result = engine->evaluate("PERMUT(3, 5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(PermutFunctionTest, NegativeNumbers) {
    auto result = engine->evaluate("PERMUT(-1, 2)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(PermutFunctionTest, NonIntegers) {
    auto result = engine->evaluate("PERMUT(5.5, 2)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(PermutFunctionTest, TooFewArguments) {
    auto result = engine->evaluate("PERMUT(5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}

TEST_F(PermutFunctionTest, TooManyArguments) {
    auto result = engine->evaluate("PERMUT(5, 2, 3)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}