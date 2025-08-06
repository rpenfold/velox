#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class IrrFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callIrr(const std::vector<Value>& args) {
        return irr(args, context);
    }
};

TEST_F(IrrFunctionTest, BasicIrrCalculation) {
    // IRR(-1000, 500, 400, 300, 200) should be around 14.28%
    auto result = callIrr({Value(-1000.0), Value(500.0), Value(400.0), Value(300.0), Value(200.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.1428, result.asNumber(), 0.001);
}

TEST_F(IrrFunctionTest, SimpleIrrCalculation) {
    // IRR(-100, 50, 60) should be around 6.54%
    auto result = callIrr({Value(-100.0), Value(50.0), Value(60.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.0654, result.asNumber(), 0.001);
}

TEST_F(IrrFunctionTest, IrrWithGuess) {
    // IRR(-1000, 500, 400, 300, 200, 0.2) with custom guess
    auto result = callIrr(
            {Value(-1000.0), Value(500.0), Value(400.0), Value(300.0), Value(200.0), Value(0.2)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.1428, result.asNumber(), 0.001);
}

TEST_F(IrrFunctionTest, NoPositiveCashFlows) {
    // IRR(-1000, -500, -400) should return error
    auto result = callIrr({Value(-1000.0), Value(-500.0), Value(-400.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IrrFunctionTest, NoNegativeCashFlows) {
    // IRR(1000, 500, 400) should return error
    auto result = callIrr({Value(1000.0), Value(500.0), Value(400.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IrrFunctionTest, InvalidArguments) {
    // No arguments
    auto result1 = callIrr({});
    EXPECT_TRUE(result1.isError());
}

TEST_F(IrrFunctionTest, ErrorPropagation) {
    auto result = callIrr({Value::error(ErrorType::VALUE_ERROR), Value(500.0), Value(400.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}