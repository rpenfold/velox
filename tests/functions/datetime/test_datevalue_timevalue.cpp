#include <gtest/gtest.h>
#include <chrono>
#include <ctime>
#include <string>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class DateTimeValueTest : public ::testing::Test {
  protected:
    Context context;
};

TEST_F(DateTimeValueTest, DATEVALUE_Parses_ISO) {
    auto result = datevalue({Value(std::string("2024-03-15"))}, context);
    ASSERT_TRUE(result.isDate());
    auto tt = std::chrono::system_clock::to_time_t(result.asDate());
    auto tm = *std::localtime(&tt);
    EXPECT_EQ(2024, tm.tm_year + 1900);
    EXPECT_EQ(3, tm.tm_mon + 1);
    EXPECT_EQ(15, tm.tm_mday);
}

TEST_F(DateTimeValueTest, DATEVALUE_Parses_Slashed) {
    auto result = datevalue({Value(std::string("12/31/2023"))}, context);
    ASSERT_TRUE(result.isDate());
    auto tt = std::chrono::system_clock::to_time_t(result.asDate());
    auto tm = *std::localtime(&tt);
    EXPECT_EQ(2023, tm.tm_year + 1900);
    EXPECT_EQ(12, tm.tm_mon + 1);
    EXPECT_EQ(31, tm.tm_mday);
}

TEST_F(DateTimeValueTest, DATEVALUE_Invalid) {
    auto result = datevalue({Value(std::string("not a date"))}, context);
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(DateTimeValueTest, TIMEVALUE_Parses_24h) {
    auto result = timevalue({Value(std::string("14:30:15"))}, context);
    ASSERT_TRUE(result.isNumber());
    double fraction = result.asNumber();
    // 14:30:15 in seconds
    int seconds = 14*3600 + 30*60 + 15;
    double expected = static_cast<double>(seconds) / (24.0 * 3600.0);
    EXPECT_NEAR(expected, fraction, 1e-9);
}

TEST_F(DateTimeValueTest, TIMEVALUE_Parses_12h) {
    auto result = timevalue({Value(std::string("2:30 PM"))}, context);
    ASSERT_TRUE(result.isNumber());
    int seconds = 14*3600 + 30*60; // 14:30:00
    double expected = static_cast<double>(seconds) / (24.0 * 3600.0);
    EXPECT_NEAR(expected, result.asNumber(), 1e-9);
}

TEST_F(DateTimeValueTest, TIMEVALUE_Invalid) {
    auto result = timevalue({Value(std::string("25:61"))}, context);
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}