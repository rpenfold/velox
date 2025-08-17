#include <gtest/gtest.h>
#include <cmath>
#include "velox/formulas/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class PiFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callPi(const std::vector<Value>& args = {}) {
        return pi(args, context);
    }
};

// Basic functionality tests
TEST_F(PiFunctionTest, NoArguments_ReturnsPI) {
    auto result = callPi();

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(M_PI, result.asNumber());
}

TEST_F(PiFunctionTest, WithArguments_ReturnsError) {
    auto result = callPi({Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(PiFunctionTest, WithMultipleArguments_ReturnsError) {
    auto result = callPi({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Value verification tests
TEST_F(PiFunctionTest, ReturnsCorrectPiValue) {
    auto result = callPi();

    EXPECT_TRUE(result.isNumber());
    // Verify it's approximately 3.14159...
    EXPECT_NEAR(3.14159265358979323846, result.asNumber(), 1e-15);
}

TEST_F(PiFunctionTest, ConsistentResults) {
    auto result1 = callPi();
    auto result2 = callPi();

    EXPECT_TRUE(result1.isNumber());
    EXPECT_TRUE(result2.isNumber());
    EXPECT_DOUBLE_EQ(result1.asNumber(), result2.asNumber());
}