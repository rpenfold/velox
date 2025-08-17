#include <gtest/gtest.h>
#include "velox/formulas/functions.h"
#include "velox/formulas/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class NperFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callNper(const std::vector<Value>& args) {
        return nper(args, context);
    }
};

TEST_F(NperFunctionTest, BasicNperCalculation) {
    // NPER(5%, -1000, 7721.73) should be around 10
    auto result = callNper({Value(0.05), Value(-1000.0), Value(7721.73)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(10.0, result.asNumber(), 0.01);
}

TEST_F(NperFunctionTest, NperWithFutureValue) {
    // NPER(6%, -200, 1000, 500) should be around 8.52
    auto result = callNper({Value(0.06), Value(-200.0), Value(1000.0), Value(500.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(8.519756, result.asNumber(), 0.01);
}

TEST_F(NperFunctionTest, NperBeginningOfPeriod) {
    // NPER(8%, -500, 1790.69, 0, 1) - beginning of period
    auto result = callNper({Value(0.08), Value(-500.0), Value(1790.69), Value(0.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(4.0, result.asNumber(), 0.01);
}

TEST_F(NperFunctionTest, ZeroRate) {
    // NPER(0%, -100, 500) should be 5
    auto result = callNper({Value(0.0), Value(-100.0), Value(500.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(5.0, result.asNumber(), 0.01);
}

TEST_F(NperFunctionTest, ZeroRateZeroPayment) {
    // NPER(0%, 0, 500) should return error (division by zero)
    auto result = callNper({Value(0.0), Value(0.0), Value(500.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(NperFunctionTest, NegativeResult) {
    // NPER(10%, -100, -500) returns negative periods (mathematically valid)
    auto result = callNper({Value(0.1), Value(-100.0), Value(-500.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-4.254164, result.asNumber(), 0.001);
}

TEST_F(NperFunctionTest, InvalidArguments) {
    // Too few arguments
    auto result1 = callNper({Value(0.05), Value(-1000.0)});
    EXPECT_TRUE(result1.isError());

    // Too many arguments
    auto result2 = callNper(
            {Value(0.05), Value(-1000.0), Value(7721.73), Value(0.0), Value(0.0), Value(0.0)});
    EXPECT_TRUE(result2.isError());

    // Invalid type value
    auto result3 = callNper({Value(0.05), Value(-1000.0), Value(7721.73), Value(0.0), Value(2.0)});
    EXPECT_TRUE(result3.isError());
}

TEST_F(NperFunctionTest, ErrorPropagation) {
    auto result = callNper({Value::error(ErrorType::VALUE_ERROR), Value(-1000.0), Value(7721.73)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}