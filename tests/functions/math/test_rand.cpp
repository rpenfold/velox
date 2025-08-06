#include <gtest/gtest.h>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class RandFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callRand(const std::vector<Value>& args = {}) {
        return rand_function(args, context);
    }
};

// Argument validation tests
TEST_F(RandFunctionTest, NoArguments_ReturnsRandomNumber) {
    auto result = callRand();

    EXPECT_TRUE(result.isNumber());
    EXPECT_GE(result.asNumber(), 0.0);
    EXPECT_LT(result.asNumber(), 1.0);
}

TEST_F(RandFunctionTest, WithArguments_ReturnsError) {
    auto result = callRand({Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(RandFunctionTest, WithMultipleArguments_ReturnsError) {
    auto result = callRand({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Randomness tests
TEST_F(RandFunctionTest, MultipleCalls_ReturnDifferentValues) {
    auto result1 = callRand();
    auto result2 = callRand();
    auto result3 = callRand();

    EXPECT_TRUE(result1.isNumber());
    EXPECT_TRUE(result2.isNumber());
    EXPECT_TRUE(result3.isNumber());

    // It's extremely unlikely that all three random numbers are the same
    bool allSame =
            (result1.asNumber() == result2.asNumber() && result2.asNumber() == result3.asNumber());
    EXPECT_FALSE(allSame);
}

TEST_F(RandFunctionTest, RangeValidation_AlwaysBetweenZeroAndOne) {
    // Test multiple random numbers to ensure they're in range
    for (int i = 0; i < 100; ++i) {
        auto result = callRand();
        EXPECT_TRUE(result.isNumber());
        EXPECT_GE(result.asNumber(), 0.0);
        EXPECT_LT(result.asNumber(), 1.0);
    }
}

TEST_F(RandFunctionTest, Distribution_CoversRange) {
    // Generate many random numbers and check they cover different parts of range
    bool hasLow = false;   // < 0.33
    bool hasMid = false;   // 0.33 - 0.66
    bool hasHigh = false;  // > 0.66

    for (int i = 0; i < 1000; ++i) {
        auto result = callRand();
        EXPECT_TRUE(result.isNumber());

        double value = result.asNumber();
        if (value < 0.33)
            hasLow = true;
        else if (value < 0.66)
            hasMid = true;
        else
            hasHigh = true;

        // If we've seen all ranges, we can break early
        if (hasLow && hasMid && hasHigh)
            break;
    }

    // With 1000 samples, we should see values in all ranges
    EXPECT_TRUE(hasLow);
    EXPECT_TRUE(hasMid);
    EXPECT_TRUE(hasHigh);
}