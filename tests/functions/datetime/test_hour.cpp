#include <gtest/gtest.h>
#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class HourFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callHour(const std::vector<Value>& args) {
        return hour(args, context);
    }

    // Helper function to create a date with time
    Value::DateType makeDateTime(int year, int month, int day, int hour, int min, int sec) {
        std::tm time_info = {};
        time_info.tm_year = year - 1900;
        time_info.tm_mon = month - 1;
        time_info.tm_mday = day;
        time_info.tm_hour = hour;
        time_info.tm_min = min;
        time_info.tm_sec = sec;
        time_info.tm_isdst = -1;

        std::time_t time_val = std::mktime(&time_info);
        return std::chrono::system_clock::from_time_t(time_val);
    }
};

// Basic functionality tests with dates
TEST_F(HourFunctionTest, DateWithMidnight_ReturnsZero) {
    auto date_val = makeDateTime(2023, 12, 25, 0, 0, 0);
    auto result = callHour({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(HourFunctionTest, DateWithNoon_ReturnsTwelve) {
    auto date_val = makeDateTime(2023, 12, 25, 12, 30, 45);
    auto result = callHour({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(12.0, result.asNumber());
}

TEST_F(HourFunctionTest, DateWithEvening_ReturnsCorrectHour) {
    auto date_val = makeDateTime(2023, 12, 25, 23, 59, 59);
    auto result = callHour({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(23.0, result.asNumber());
}

// Basic functionality tests with time fractions
TEST_F(HourFunctionTest, TimeFractionMidnight_ReturnsZero) {
    // 0.0 represents midnight
    auto result = callHour({Value(0.0)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(HourFunctionTest, TimeFractionNoon_ReturnsTwelve) {
    // 0.5 represents noon (12:00:00)
    auto result = callHour({Value(0.5)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(12.0, result.asNumber());
}

TEST_F(HourFunctionTest, TimeFractionSpecific_ReturnsCorrectHour) {
    // 12:30:45 = 45045 seconds = 45045/86400 ≈ 0.521354167
    double time_fraction = 45045.0 / 86400.0;
    auto result = callHour({Value(time_fraction)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(12.0, result.asNumber());
}

TEST_F(HourFunctionTest, TimeFractionAlmostMidnight_ReturnsCorrectHour) {
    // 23:59:59 = 86399 seconds = 86399/86400
    double time_fraction = 86399.0 / 86400.0;
    auto result = callHour({Value(time_fraction)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(23.0, result.asNumber());
}

// Time fraction with date part (greater than 1.0)
TEST_F(HourFunctionTest, TimeFractionWithDatePart_ExtractsFractionalPart) {
    // 1.5 means 1 day + 0.5 day (12:00:00)
    auto result = callHour({Value(1.5)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(12.0, result.asNumber());
}

TEST_F(HourFunctionTest, NegativeTimeFraction_HandlesCorrectly) {
    // -0.25 should be treated as 0.75 (18:00:00)
    auto result = callHour({Value(-0.25)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(18.0, result.asNumber());
}

// Argument validation tests
TEST_F(HourFunctionTest, TooManyArguments_ReturnsError) {
    auto date_val = makeDateTime(2023, 12, 25, 12, 0, 0);
    auto result = callHour({Value(date_val), Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(HourFunctionTest, NoArguments_ReturnsError) {
    auto result = callHour({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Invalid input tests
TEST_F(HourFunctionTest, NonNumericTextArgument_ReturnsError) {
    auto result = callHour({Value("hello")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(HourFunctionTest, BooleanTrueArgument_ConvertsToNumber) {
    // Boolean true converts to 1.0, which is a time fraction
    auto result = callHour({Value(true)});

    EXPECT_TRUE(result.isNumber());
    // 1.0 as time fraction wraps around (1.0 - 1.0 = 0.0 = midnight)
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(HourFunctionTest, BooleanFalseArgument_ConvertsToNumber) {
    // Boolean false converts to 0.0, which is midnight
    auto result = callHour({Value(false)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

// Error propagation tests
TEST_F(HourFunctionTest, ErrorArgument_PropagatesError) {
    auto result = callHour({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

// Edge cases with various time values
TEST_F(HourFunctionTest, OnePM_ReturnsThirteen) {
    auto date_val = makeDateTime(2023, 12, 25, 13, 0, 0);
    auto result = callHour({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(13.0, result.asNumber());
}

TEST_F(HourFunctionTest, SixAM_ReturnsSix) {
    auto date_val = makeDateTime(2023, 12, 25, 6, 45, 30);
    auto result = callHour({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(HourFunctionTest, TimeFractionQuarterDay_ReturnsSix) {
    // 0.25 of a day = 6:00:00
    auto result = callHour({Value(0.25)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(6.0, result.asNumber());
}

TEST_F(HourFunctionTest, TimeFractionThreeQuarterDay_ReturnsEighteen) {
    // 0.75 of a day = 18:00:00
    auto result = callHour({Value(0.75)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(18.0, result.asNumber());
}