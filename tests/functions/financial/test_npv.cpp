#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class NpvFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callNpv(const std::vector<Value>& args) {
        return npv(args, context);
    }
};

TEST_F(NpvFunctionTest, BasicNpvCalculation) {
    // NPV(10%, -1000, 500, 400, 300, 200)
    auto result = callNpv(
            {Value(0.1), Value(-1000.0), Value(500.0), Value(400.0), Value(300.0), Value(200.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(133.746453, result.asNumber(), 0.01);
}

TEST_F(NpvFunctionTest, SimpleNpvCalculation) {
    // NPV(5%, 100, 200) should be around 276.64
    auto result = callNpv({Value(0.05), Value(100.0), Value(200.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(276.643991, result.asNumber(), 0.01);
}

TEST_F(NpvFunctionTest, NegativeNpv) {
    // NPV(15%, -1000, 100, 100, 100) should be negative
    auto result = callNpv({Value(0.15), Value(-1000.0), Value(100.0), Value(100.0), Value(100.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_LT(result.asNumber(), 0);
    EXPECT_NEAR(-671.023903, result.asNumber(), 0.01);
}

TEST_F(NpvFunctionTest, ZeroRate) {
    // NPV(0%, 100, 200, 300) should be 600
    auto result = callNpv({Value(0.0), Value(100.0), Value(200.0), Value(300.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(600.0, result.asNumber(), 0.01);
}

TEST_F(NpvFunctionTest, InvalidArguments) {
    // Too few arguments
    auto result1 = callNpv({Value(0.1)});
    EXPECT_TRUE(result1.isError());

    // Only rate provided
    auto result2 = callNpv({Value(0.1)});
    EXPECT_TRUE(result2.isError());
}

TEST_F(NpvFunctionTest, ErrorPropagation) {
    auto result = callNpv({Value::error(ErrorType::VALUE_ERROR), Value(100.0), Value(200.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}