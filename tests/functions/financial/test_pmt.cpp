#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class PmtFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callPmt(const std::vector<Value>& args) {
        return pmt(args, context);
    }
};

TEST_F(PmtFunctionTest, BasicPmtCalculation) {
    // PMT(5%, 10, 10000) should be around -1295.05
    auto result = callPmt({Value(0.05), Value(10.0), Value(10000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-1295.05, result.asNumber(), 0.01);
}

TEST_F(PmtFunctionTest, PmtWithFutureValue) {
    // PMT(6%, 5, 5000, 1000) should be around -1364.38
    auto result = callPmt({Value(0.06), Value(5.0), Value(5000.0), Value(1000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-1364.378403, result.asNumber(), 0.01);
}

TEST_F(PmtFunctionTest, PmtBeginningOfPeriod) {
    // PMT(8%, 4, 5000, 0, 1) - beginning of period
    auto result = callPmt({Value(0.08), Value(4.0), Value(5000.0), Value(0.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-1397.781502, result.asNumber(), 0.01);
}

TEST_F(PmtFunctionTest, ZeroRate) {
    // PMT(0%, 5, 1000) should be -200
    auto result = callPmt({Value(0.0), Value(5.0), Value(1000.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(-200.0, result.asNumber(), 0.01);
}

TEST_F(PmtFunctionTest, InvalidArguments) {
    // Too few arguments
    auto result1 = callPmt({Value(0.05), Value(10.0)});
    EXPECT_TRUE(result1.isError());

    // Too many arguments
    auto result2 =
            callPmt({Value(0.05), Value(10.0), Value(10000.0), Value(0.0), Value(0.0), Value(0.0)});
    EXPECT_TRUE(result2.isError());

    // Invalid type value
    auto result3 = callPmt({Value(0.05), Value(10.0), Value(10000.0), Value(0.0), Value(2.0)});
    EXPECT_TRUE(result3.isError());
}

TEST_F(PmtFunctionTest, ErrorPropagation) {
    auto result = callPmt({Value::error(ErrorType::VALUE_ERROR), Value(10.0), Value(10000.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}