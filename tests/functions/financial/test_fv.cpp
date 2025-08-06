#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class FvFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callFv(const std::vector<Value>& args) {
        return fv(args, context);
    }
};

TEST_F(FvFunctionTest, BasicFvCalculation) {
    // FV(5%, 10, -1000) should be around 12578.95
    auto result = callFv({Value(0.05), Value(10.0), Value(-1000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(12578.95, result.asNumber(), 0.01);
}

TEST_F(FvFunctionTest, FvWithPresentValue) {
    // FV(6%, 5, -200, -1000) should be around 2673.01
    auto result = callFv({Value(0.06), Value(5.0), Value(-200.0), Value(-1000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2673.01, result.asNumber(), 0.01);
}

TEST_F(FvFunctionTest, FvBeginningOfPeriod) {
    // FV(8%, 4, -500, 0, 1) - beginning of period
    auto result = callFv({Value(0.08), Value(4.0), Value(-500.0), Value(0.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(2330.88, result.asNumber(), 0.01);
}

TEST_F(FvFunctionTest, ZeroRate) {
    // FV(0%, 5, -100) should be 500
    auto result = callFv({Value(0.0), Value(5.0), Value(-100.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(500.0, result.asNumber(), 0.01);
}

TEST_F(FvFunctionTest, InvalidArguments) {
    // Too few arguments
    auto result1 = callFv({Value(0.05), Value(10.0)});
    EXPECT_TRUE(result1.isError());

    // Too many arguments
    auto result2 =
            callFv({Value(0.05), Value(10.0), Value(-1000.0), Value(0.0), Value(0.0), Value(0.0)});
    EXPECT_TRUE(result2.isError());

    // Invalid type value
    auto result3 = callFv({Value(0.05), Value(10.0), Value(-1000.0), Value(0.0), Value(2.0)});
    EXPECT_TRUE(result3.isError());
}

TEST_F(FvFunctionTest, ErrorPropagation) {
    auto result = callFv({Value::error(ErrorType::VALUE_ERROR), Value(10.0), Value(-1000.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}