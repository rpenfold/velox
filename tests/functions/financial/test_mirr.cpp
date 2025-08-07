#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/types.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class MirrFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callMirr(const std::vector<Value>& args) {
        return mirr(args, context);
    }
};

TEST_F(MirrFunctionTest, BasicMirrCalculation) {
    // MIRR(-1000, 500, 400, 300, 200, 10%, 12%) should be around 14.86%
    auto result = callMirr({Value(-1000.0), Value(500.0), Value(400.0), Value(300.0), Value(200.0),
                            Value(0.1), Value(0.12)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.148554, result.asNumber(), 0.001);
}

TEST_F(MirrFunctionTest, SimpleMirrCalculation) {
    // MIRR(-100, 50, 60, 8%, 10%) should be around 7.24%
    auto result = callMirr({Value(-100.0), Value(50.0), Value(60.0), Value(0.08), Value(0.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.072381, result.asNumber(), 0.001);
}

TEST_F(MirrFunctionTest, MirrSameRates) {
    // MIRR(-1000, 500, 400, 300, 200, 10%, 10%) with same rates
    auto result = callMirr({Value(-1000.0), Value(500.0), Value(400.0), Value(300.0), Value(200.0),
                            Value(0.1), Value(0.1)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_NEAR(0.138400, result.asNumber(), 0.001);
}

TEST_F(MirrFunctionTest, MirrNoOutflows) {
    // MIRR(500, 400, 300, 200, 10%, 12%) should return error (no negative cash flows)
    auto result = callMirr(
            {Value(500.0), Value(400.0), Value(300.0), Value(200.0), Value(0.1), Value(0.12)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MirrFunctionTest, MirrNoInflows) {
    // MIRR(-1000, -500, -400, 10%, 12%) should return error (no positive cash flows)
    auto result = callMirr({Value(-1000.0), Value(-500.0), Value(-400.0), Value(0.1), Value(0.12)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(MirrFunctionTest, InvalidArguments) {
    // Too few arguments
    auto result1 = callMirr({Value(-1000.0), Value(0.1)});
    EXPECT_TRUE(result1.isError());

    // Need at least one cash flow and two rates
    auto result2 = callMirr({Value(0.1), Value(0.12)});
    EXPECT_TRUE(result2.isError());
}

TEST_F(MirrFunctionTest, ErrorPropagation) {
    auto result =
            callMirr({Value::error(ErrorType::VALUE_ERROR), Value(500.0), Value(0.1), Value(0.12)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}