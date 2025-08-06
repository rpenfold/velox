#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class NowFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callNow(const std::vector<Value>& args = {}) {
        return now(args, context);
    }
};

// Basic functionality tests
TEST_F(NowFunctionTest, NoArguments_ReturnsDate) {
    auto result = callNow();

    EXPECT_TRUE(result.isDate());
}

TEST_F(NowFunctionTest, WithArguments_ReturnsError) {
    auto result = callNow({Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(NowFunctionTest, WithMultipleArguments_ReturnsError) {
    auto result = callNow({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Value verification tests
TEST_F(NowFunctionTest, ReturnsCurrentTime) {
    auto before = std::chrono::system_clock::now();
    auto result = callNow();
    auto after = std::chrono::system_clock::now();

    EXPECT_TRUE(result.isDate());

    auto result_time = result.asDate();
    EXPECT_GE(result_time, before);
    EXPECT_LE(result_time, after);
}

TEST_F(NowFunctionTest, ConsecutiveCalls_IncreasingTime) {
    auto result1 = callNow();
    // Small delay to ensure different timestamps
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    auto result2 = callNow();

    EXPECT_TRUE(result1.isDate());
    EXPECT_TRUE(result2.isDate());
    EXPECT_LE(result1.asDate(), result2.asDate());
}

// Error handling tests
TEST_F(NowFunctionTest, WithErrorArgument_ReturnsError) {
    auto result = callNow({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    // noArgFunction template returns VALUE_ERROR when args count is wrong
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}