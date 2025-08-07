#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class PvFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callPv(const std::vector<Value>& args) {
        return pv(args, context);
    }
};

TEST_F(PvFunctionTest, BasicPvCalculation) {
    // PV(5%, 10, -1000) should be around 7721.73
    auto result = callPv({Value(0.05), Value(10.0), Value(-1000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(7721.73, result.asNumber(), 0.01);
}

TEST_F(PvFunctionTest, PvWithFutureValue) {
    // PV(6%, 5, -200, -1000) should be around 1589.56
    auto result = callPv({Value(0.06), Value(5.0), Value(-200.0), Value(-1000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1589.731, result.asNumber(), 0.01);
}

TEST_F(PvFunctionTest, PvBeginningOfPeriod) {
    // PV(8%, 4, -500, 0, 1) - beginning of period
    auto result = callPv({Value(0.08), Value(4.0), Value(-500.0), Value(0.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(1788.548, result.asNumber(), 0.01);
}

TEST_F(PvFunctionTest, ZeroRate) {
    // PV(0%, 5, -100) should be 500
    auto result = callPv({Value(0.0), Value(5.0), Value(-100.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(500.0, result.asNumber(), 0.01);
}

TEST_F(PvFunctionTest, InvalidArguments) {
    // Too few arguments
    auto result1 = callPv({Value(0.05), Value(10.0)});
    EXPECT_TRUE(result1.isError());

    // Too many arguments
    auto result2 =
            callPv({Value(0.05), Value(10.0), Value(-1000.0), Value(0.0), Value(0.0), Value(0.0)});
    EXPECT_TRUE(result2.isError());

    // Invalid type value
    auto result3 = callPv({Value(0.05), Value(10.0), Value(-1000.0), Value(0.0), Value(2.0)});
    EXPECT_TRUE(result3.isError());
}

TEST_F(PvFunctionTest, ErrorPropagation) {
    auto result = callPv({Value::error(ErrorType::VALUE_ERROR), Value(10.0), Value(-1000.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}