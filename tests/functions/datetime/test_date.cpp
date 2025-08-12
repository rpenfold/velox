#include <gtest/gtest.h>
#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class DateFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callDate(const std::vector<Value>& args) {
        return date(args, context);
    }
};

// Basic functionality tests
TEST_F(DateFunctionTest, ValidDate_ReturnsDate) {
    auto result = callDate({Value(2023.0), Value(12.0), Value(25.0)});

    EXPECT_TRUE(result.isDate());

    auto date_val = result.asDate();
    auto time_t = std::chrono::system_clock::to_time_t(date_val);
    auto local_tm = *std::localtime(&time_t);

    EXPECT_EQ(2023, local_tm.tm_year + 1900);
    EXPECT_EQ(12, local_tm.tm_mon + 1);
    EXPECT_EQ(25, local_tm.tm_mday);
}

TEST_F(DateFunctionTest, MinimumValidDate_ReturnsDate) {
    auto result = callDate({Value(1900.0), Value(1.0), Value(1.0)});

    EXPECT_TRUE(result.isDate());

    auto date_val = result.asDate();
    auto time_t = std::chrono::system_clock::to_time_t(date_val);
    auto local_tm = *std::localtime(&time_t);

    EXPECT_EQ(1900, local_tm.tm_year + 1900);
    EXPECT_EQ(1, local_tm.tm_mon + 1);
    EXPECT_EQ(1, local_tm.tm_mday);
}

TEST_F(DateFunctionTest, MaximumValidDate_ReturnsDate) {
    auto result = callDate({Value(2099.0), Value(12.0), Value(31.0)});

    EXPECT_TRUE(result.isDate());

    auto date_val = result.asDate();
    auto time_t = std::chrono::system_clock::to_time_t(date_val);
    auto local_tm = *std::localtime(&time_t);

    EXPECT_EQ(2099, local_tm.tm_year + 1900);
    EXPECT_EQ(12, local_tm.tm_mon + 1);
    EXPECT_EQ(31, local_tm.tm_mday);
}

// Argument validation tests
TEST_F(DateFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callDate({Value(2023.0), Value(12.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(DateFunctionTest, TooManyArguments_ReturnsError) {
    auto result = callDate({Value(2023.0), Value(12.0), Value(25.0), Value(1.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(DateFunctionTest, NoArguments_ReturnsError) {
    auto result = callDate({});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Invalid input tests
TEST_F(DateFunctionTest, NonNumericYear_ReturnsError) {
    auto result = callDate({Value("text"), Value(12.0), Value(25.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(DateFunctionTest, NonNumericMonth_ReturnsError) {
    auto result = callDate({Value(2023.0), Value("text"), Value(25.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(DateFunctionTest, NonNumericDay_ReturnsError) {
    auto result = callDate({Value(2023.0), Value(12.0), Value("text")});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Range validation tests
TEST_F(DateFunctionTest, YearTooLow_ReturnsError) {
    auto result = callDate({Value(1899.0), Value(12.0), Value(25.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(DateFunctionTest, YearTooHigh_ReturnsError) {
    auto result = callDate({Value(2100.0), Value(12.0), Value(25.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(DateFunctionTest, MonthTooLow_ReturnsError) {
    auto result = callDate({Value(2023.0), Value(0.0), Value(25.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(DateFunctionTest, MonthTooHigh_ReturnsError) {
    auto result = callDate({Value(2023.0), Value(13.0), Value(25.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(DateFunctionTest, DayTooLow_ReturnsError) {
    auto result = callDate({Value(2023.0), Value(12.0), Value(0.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

TEST_F(DateFunctionTest, DayTooHigh_ReturnsError) {
    auto result = callDate({Value(2023.0), Value(12.0), Value(32.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}

// Error propagation tests
TEST_F(DateFunctionTest, ErrorInYear_PropagatesError) {
    auto result = callDate({Value::error(ErrorType::DIV_ZERO), Value(12.0), Value(25.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(DateFunctionTest, ErrorInMonth_PropagatesError) {
    auto result = callDate({Value(2023.0), Value::error(ErrorType::DIV_ZERO), Value(25.0)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(DateFunctionTest, ErrorInDay_PropagatesError) {
    auto result = callDate({Value(2023.0), Value(12.0), Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

// Special date tests
TEST_F(DateFunctionTest, LeapYear_February29_ReturnsDate) {
    auto result = callDate({Value(2024.0), Value(2.0), Value(29.0)});

    EXPECT_TRUE(result.isDate());
}

TEST_F(DateFunctionTest, NonLeapYear_February29_HandledBySystem) {
    // Note: mktime will handle invalid dates by adjusting them
    auto result = callDate({Value(2023.0), Value(2.0), Value(29.0)});

    // The system will likely adjust this to March 1st
    EXPECT_TRUE(result.isDate());
}

TEST_F(DateFunctionTest, Edate_And_Eomonth_Basic) {
    auto start = callDate({Value(2024.0), Value(1.0), Value(31.0)});
    ASSERT_TRUE(start.isDate());
    auto next = edate({start, Value(1.0)}, context);
    EXPECT_TRUE(next.isDate());
    auto eom = eomonth({start, Value(1.0)}, context);
    EXPECT_TRUE(eom.isDate());
}