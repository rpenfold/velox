#include <gtest/gtest.h>
#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class YearFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callYear(const std::vector<Value>& args) {
        return year(args, context);
    }

    // Helper function to create a date
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

// Basic functionality tests
TEST_F(YearFunctionTest, ValidDate_ReturnsYear) {
    auto date_val = makeDate(2023, 12, 25);
    auto result = callYear({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2023.0, result.asNumber());
}

TEST_F(YearFunctionTest, MinimumYear_ReturnsCorrectYear) {
    auto date_val = makeDate(1900, 1, 1);
    auto result = callYear({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1900.0, result.asNumber());
}

TEST_F(YearFunctionTest, MaximumYear_ReturnsCorrectYear) {
    auto date_val = makeDate(2099, 12, 31);
    auto result = callYear({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2099.0, result.asNumber());
}

TEST_F(YearFunctionTest, LeapYear_ReturnsCorrectYear) {
    auto date_val = makeDate(2024, 2, 29);
    auto result = callYear({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2024.0, result.asNumber());
}

// Argument validation tests
TEST_F(YearFunctionTest, TooManyArguments_ReturnsError) {
    auto date_val = makeDate(2023, 12, 25);
    auto result = callYear({Value(date_val), Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(YearFunctionTest, NoArguments_ReturnsError) {
    auto result = callYear({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Invalid input tests
TEST_F(YearFunctionTest, NonDateArgument_ReturnsError) {
    auto result = callYear({Value(12345.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(YearFunctionTest, TextArgument_ReturnsError) {
    auto result = callYear({Value("2023-12-25")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(YearFunctionTest, BooleanArgument_ReturnsError) {
    auto result = callYear({Value(true)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Error propagation tests
TEST_F(YearFunctionTest, ErrorArgument_PropagatesError) {
    auto result = callYear({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

// Edge cases
TEST_F(YearFunctionTest, NewYearsDay_ReturnsCorrectYear) {
    auto date_val = makeDate(2023, 1, 1);
    auto result = callYear({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2023.0, result.asNumber());
}

TEST_F(YearFunctionTest, NewYearsEve_ReturnsCorrectYear) {
    auto date_val = makeDate(2023, 12, 31);
    auto result = callYear({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2023.0, result.asNumber());
}

TEST_F(YearFunctionTest, DateWithTime_ReturnsCorrectYear) {
    // Create a date with specific time components
    std::tm time_info = {};
    time_info.tm_year = 2023 - 1900;
    time_info.tm_mon = 6 - 1;  // June
    time_info.tm_mday = 15;
    time_info.tm_hour = 14;    // 2 PM
    time_info.tm_min = 30;
    time_info.tm_sec = 45;
    time_info.tm_isdst = -1;
    
    std::time_t time_val = std::mktime(&time_info);
    auto date_val = std::chrono::system_clock::from_time_t(time_val);
    
    auto result = callYear({Value(date_val)});

    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2023.0, result.asNumber());
}