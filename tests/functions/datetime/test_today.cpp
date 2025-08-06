#include <gtest/gtest.h>
#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class TodayFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callToday(const std::vector<Value>& args = {}) {
        return today(args, context);
    }
};

// Basic functionality tests
TEST_F(TodayFunctionTest, NoArguments_ReturnsDate) {
    auto result = callToday();

    EXPECT_TRUE(result.isDate());
}

TEST_F(TodayFunctionTest, WithArguments_ReturnsError) {
    auto result = callToday({Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(TodayFunctionTest, WithMultipleArguments_ReturnsError) {
    auto result = callToday({Value(1.0), Value(2.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Value verification tests
TEST_F(TodayFunctionTest, ReturnsCurrentDate) {
    auto result = callToday();

    EXPECT_TRUE(result.isDate());

    // Check that it's today's date at midnight
    auto result_time = result.asDate();
    auto time_t = std::chrono::system_clock::to_time_t(result_time);
    auto local_tm = *std::localtime(&time_t);

    // Should be midnight (00:00:00)
    EXPECT_EQ(0, local_tm.tm_hour);
    EXPECT_EQ(0, local_tm.tm_min);
    EXPECT_EQ(0, local_tm.tm_sec);

    // Should be today's date
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::localtime(&now_time_t);

    EXPECT_EQ(now_tm.tm_year, local_tm.tm_year);
    EXPECT_EQ(now_tm.tm_mon, local_tm.tm_mon);
    EXPECT_EQ(now_tm.tm_mday, local_tm.tm_mday);
}

TEST_F(TodayFunctionTest, ConsistentResults_SameDay) {
    auto result1 = callToday();
    auto result2 = callToday();

    EXPECT_TRUE(result1.isDate());
    EXPECT_TRUE(result2.isDate());
    EXPECT_EQ(result1.asDate(), result2.asDate());
}

// Error handling tests
TEST_F(TodayFunctionTest, WithErrorArgument_ReturnsError) {
    auto result = callToday({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    // noArgFunction template returns VALUE_ERROR when args count is wrong
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}