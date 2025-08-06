#include <gtest/gtest.h>
#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class WeekdayFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callWeekday(const std::vector<Value>& args) {
        return weekday(args, context);
    }

    // Helper function to create a date for a known weekday
    // January 1, 2023 was a Sunday
    Value::DateType makeDate(int year, int month, int day) {
        std::tm time_info = {};
        time_info.tm_year = year - 1900;
        time_info.tm_mon = month - 1;
        time_info.tm_mday = day;
        time_info.tm_hour = 0;
        time_info.tm_min = 0;
        time_info.tm_sec = 0;
        time_info.tm_isdst = -1;

        std::time_t time_val = std::mktime(&time_info);
        return std::chrono::system_clock::from_time_t(time_val);
    }
};

// Basic functionality tests with default return type (1)
TEST_F(WeekdayFunctionTest, Sunday_DefaultType_ReturnsOne) {
    // January 1, 2023 was a Sunday
    auto date_val = makeDate(2023, 1, 1);
    auto result = callWeekday({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(WeekdayFunctionTest, Monday_DefaultType_ReturnsTwo) {
    // January 2, 2023 was a Monday
    auto date_val = makeDate(2023, 1, 2);
    auto result = callWeekday({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(WeekdayFunctionTest, Saturday_DefaultType_ReturnsSeven) {
    // January 7, 2023 was a Saturday
    auto date_val = makeDate(2023, 1, 7);
    auto result = callWeekday({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

// Tests with explicit return type 1 (Sunday=1)
TEST_F(WeekdayFunctionTest, Sunday_Type1_ReturnsOne) {
    auto date_val = makeDate(2023, 1, 1);
    auto result = callWeekday({Value(date_val), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(WeekdayFunctionTest, Monday_Type1_ReturnsTwo) {
    auto date_val = makeDate(2023, 1, 2);
    auto result = callWeekday({Value(date_val), Value(1.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

// Tests with return type 2 (Monday=1)
TEST_F(WeekdayFunctionTest, Monday_Type2_ReturnsOne) {
    auto date_val = makeDate(2023, 1, 2);
    auto result = callWeekday({Value(date_val), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(WeekdayFunctionTest, Sunday_Type2_ReturnsSeven) {
    auto date_val = makeDate(2023, 1, 1);
    auto result = callWeekday({Value(date_val), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());
}

TEST_F(WeekdayFunctionTest, Tuesday_Type2_ReturnsTwo) {
    auto date_val = makeDate(2023, 1, 3);
    auto result = callWeekday({Value(date_val), Value(2.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

// Tests with return type 3 (Monday=0)
TEST_F(WeekdayFunctionTest, Monday_Type3_ReturnsZero) {
    auto date_val = makeDate(2023, 1, 2);
    auto result = callWeekday({Value(date_val), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(WeekdayFunctionTest, Sunday_Type3_ReturnsSix) {
    auto date_val = makeDate(2023, 1, 1);
    auto result = callWeekday({Value(date_val), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(WeekdayFunctionTest, Saturday_Type3_ReturnsFive) {
    auto date_val = makeDate(2023, 1, 7);
    auto result = callWeekday({Value(date_val), Value(3.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

// Argument validation tests
TEST_F(WeekdayFunctionTest, TooManyArguments_ReturnsError) {
    auto date_val = makeDate(2023, 1, 1);
    auto result = callWeekday({Value(date_val), Value(1.0), Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(WeekdayFunctionTest, NoArguments_ReturnsError) {
    auto result = callWeekday({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Invalid input tests
TEST_F(WeekdayFunctionTest, NonDateArgument_ReturnsError) {
    auto result = callWeekday({Value(12345.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(WeekdayFunctionTest, TextArgument_ReturnsError) {
    auto result = callWeekday({Value("2023-01-01")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(WeekdayFunctionTest, InvalidReturnType_ReturnsError) {
    auto date_val = makeDate(2023, 1, 1);
    auto result = callWeekday({Value(date_val), Value(4.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(WeekdayFunctionTest, ZeroReturnType_ReturnsError) {
    auto date_val = makeDate(2023, 1, 1);
    auto result = callWeekday({Value(date_val), Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(WeekdayFunctionTest, NonNumericReturnType_ReturnsError) {
    auto date_val = makeDate(2023, 1, 1);
    auto result = callWeekday({Value(date_val), Value("text")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Error propagation tests
TEST_F(WeekdayFunctionTest, ErrorInDate_PropagatesError) {
    auto result = callWeekday({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(WeekdayFunctionTest, ErrorInReturnType_PropagatesError) {
    auto date_val = makeDate(2023, 1, 1);
    auto result = callWeekday({Value(date_val), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

// Edge cases
TEST_F(WeekdayFunctionTest, LeapYearDate_ReturnsCorrectWeekday) {
    // February 29, 2024 (leap year)
    auto date_val = makeDate(2024, 2, 29);
    auto result = callWeekday({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    // Should return a valid weekday (1-7)
    EXPECT_GE(result.asNumber(), 1.0);
    EXPECT_LE(result.asNumber(), 7.0);
}

TEST_F(WeekdayFunctionTest, DateWithTime_IgnoresTime) {
    // Create date with time components
    std::tm time_info = {};
    time_info.tm_year = 2023 - 1900;
    time_info.tm_mon = 1 - 1;  // January
    time_info.tm_mday = 1;
    time_info.tm_hour = 14;  // 2 PM
    time_info.tm_min = 30;
    time_info.tm_sec = 45;
    time_info.tm_isdst = -1;

    std::time_t time_val = std::mktime(&time_info);
    auto date_val = std::chrono::system_clock::from_time_t(time_val);

    auto result = callWeekday({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());  // Should still be Sunday (1)
}