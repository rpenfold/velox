#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;

class SumproductFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        engine = std::make_unique<FormulaEngine>();
    }

    std::unique_ptr<FormulaEngine> engine;
};

TEST_F(SumproductFunctionTest, SimpleMultiplication) {
    auto result = engine->evaluate("SUMPRODUCT(2, 3, 4)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 24.0);
}

TEST_F(SumproductFunctionTest, TwoNumbers) {
    auto result = engine->evaluate("SUMPRODUCT(5, 6)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 30.0);
}

TEST_F(SumproductFunctionTest, SingleNumber) {
    auto result = engine->evaluate("SUMPRODUCT(7)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 7.0);
}

TEST_F(SumproductFunctionTest, WithZero) {
    auto result = engine->evaluate("SUMPRODUCT(5, 0, 3)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 0.0);
}

TEST_F(SumproductFunctionTest, NegativeNumbers) {
    auto result = engine->evaluate("SUMPRODUCT(-2, 3)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), -6.0);
}

TEST_F(SumproductFunctionTest, DecimalNumbers) {
    auto result = engine->evaluate("SUMPRODUCT(1.5, 2.0)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 3.0);
}

TEST_F(SumproductFunctionTest, EmptyArguments) {
    auto result = engine->evaluate("SUMPRODUCT()");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}