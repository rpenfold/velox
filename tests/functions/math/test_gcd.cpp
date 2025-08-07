#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;

class GcdFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        engine = std::make_unique<FormulaEngine>();
    }

    std::unique_ptr<FormulaEngine> engine;
};

TEST_F(GcdFunctionTest, TwoPositiveNumbers) {
    auto result = engine->evaluate("GCD(12, 8)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 4.0);
}

TEST_F(GcdFunctionTest, MultipleNumbers) {
    auto result = engine->evaluate("GCD(24, 36, 48)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 12.0);
}

TEST_F(GcdFunctionTest, OneNumber) {
    auto result = engine->evaluate("GCD(15)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 15.0);
}

TEST_F(GcdFunctionTest, WithZero) {
    auto result = engine->evaluate("GCD(0, 5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 5.0);
}

TEST_F(GcdFunctionTest, NegativeNumbers) {
    auto result = engine->evaluate("GCD(-12, 8)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 4.0);
}

TEST_F(GcdFunctionTest, CoprimeNumbers) {
    auto result = engine->evaluate("GCD(7, 13)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 1.0);
}

TEST_F(GcdFunctionTest, EmptyArguments) {
    auto result = engine->evaluate("GCD()");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}