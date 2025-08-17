#include <gtest/gtest.h>
#include "velox/formulas/functions.h"
#include "velox/formulas/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class RateFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callRate(const std::vector<Value>& args) {
        return rate(args, context);
    }
};

TEST_F(RateFunctionTest, BasicRateCalculation) {
    // RATE(10, -1000, 7721.73) should be around 5%
    auto result = callRate({Value(10.0), Value(-1000.0), Value(7721.73)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.05, result.asNumber(), 0.001);
}

TEST_F(RateFunctionTest, RateWithFutureValue) {
    // RATE(5, -200, 800, 0) should be around 7.93%
    auto result = callRate({Value(5.0), Value(-200.0), Value(800.0), Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.07931, result.asNumber(), 0.001);
}

TEST_F(RateFunctionTest, RateBeginningOfPeriod) {
    // RATE(4, -500, 1790.69, 0, 1) - beginning of period
    auto result = callRate({Value(4.0), Value(-500.0), Value(1790.69), Value(0.0), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.08, result.asNumber(), 0.001);
}

TEST_F(RateFunctionTest, RateWithCustomGuess) {
    // RATE(10, -1000, 7721.73, 0, 0, 0.1) with custom guess
    auto result = callRate(
            {Value(10.0), Value(-1000.0), Value(7721.73), Value(0.0), Value(0.0), Value(0.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.05, result.asNumber(), 0.001);
}

TEST_F(RateFunctionTest, InvalidArguments) {
    // Too few arguments
    auto result1 = callRate({Value(10.0), Value(-1000.0)});
    EXPECT_TRUE(result1.isError());

    // Too many arguments
    auto result2 = callRate({Value(10.0), Value(-1000.0), Value(7721.73), Value(0.0), Value(0.0),
                             Value(0.1), Value(0.0)});
    EXPECT_TRUE(result2.isError());

    // Invalid type value
    auto result3 = callRate({Value(10.0), Value(-1000.0), Value(7721.73), Value(0.0), Value(2.0)});
    EXPECT_TRUE(result3.isError());
}

TEST_F(RateFunctionTest, NoConvergence) {
    // Impossible conditions that won't converge
    auto result = callRate({Value(10.0), Value(1000.0), Value(1000.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RateFunctionTest, ErrorPropagation) {
    auto result = callRate({Value::error(ErrorType::VALUE_ERROR), Value(-1000.0), Value(7721.73)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}