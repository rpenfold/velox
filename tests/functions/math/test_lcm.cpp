#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;

class LcmFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        engine = std::make_unique<FormulaEngine>();
    }

    std::unique_ptr<FormulaEngine> engine;
};

TEST_F(LcmFunctionTest, TwoPositiveNumbers) {
    auto result = engine->evaluate("LCM(4, 6)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 12.0);
}

TEST_F(LcmFunctionTest, MultipleNumbers) {
    auto result = engine->evaluate("LCM(2, 3, 4)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 12.0);
}

TEST_F(LcmFunctionTest, OneNumber) {
    auto result = engine->evaluate("LCM(15)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 15.0);
}

TEST_F(LcmFunctionTest, WithZero) {
    auto result = engine->evaluate("LCM(0, 5)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 0.0);
}

TEST_F(LcmFunctionTest, NegativeNumbers) {
    auto result = engine->evaluate("LCM(-4, 6)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 12.0);
}

TEST_F(LcmFunctionTest, CoprimeNumbers) {
    auto result = engine->evaluate("LCM(7, 13)");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_DOUBLE_EQ(result.getValue().asNumber(), 91.0);
}

TEST_F(LcmFunctionTest, EmptyArguments) {
    auto result = engine->evaluate("LCM()");
    ASSERT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
}