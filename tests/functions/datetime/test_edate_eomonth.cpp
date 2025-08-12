#include <gtest/gtest.h>
#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

using namespace xl_formula;
using namespace xl_formula::functions::builtin;

class EdateEomonthTest : public ::testing::Test {
  protected:
    Context context;
};

TEST_F(EdateEomonthTest, EDATE_Basic_PositiveMonths) {
    // 2023-01-31 + 1 month => 2023-02-28 (clamped)
    // Build start date via DATE
    auto start = date({Value(2023.0), Value(1.0), Value(31.0)}, context);
    ASSERT_TRUE(start.isDate());

    auto result = edate({start, Value(1.0)}, context);
    ASSERT_TRUE(result.isDate());

    auto tp = result.asDate();
    auto tt = std::chrono::system_clock::to_time_t(tp);
    auto tm = *std::localtime(&tt);
    EXPECT_EQ(2023, tm.tm_year + 1900);
    EXPECT_EQ(2, tm.tm_mon + 1);
    EXPECT_EQ(28, tm.tm_mday);
}

TEST_F(EdateEomonthTest, EDATE_NegativeMonths) {
    auto start = date({Value(2024.0), Value(3.0), Value(15.0)}, context);
    ASSERT_TRUE(start.isDate());

    auto result = edate({start, Value(-2.0)}, context);
    ASSERT_TRUE(result.isDate());

    auto tp = result.asDate();
    auto tt = std::chrono::system_clock::to_time_t(tp);
    auto tm = *std::localtime(&tt);
    EXPECT_EQ(2024, tm.tm_year + 1900);
    EXPECT_EQ(1, tm.tm_mon + 1);
    EXPECT_EQ(15, tm.tm_mday);
}

TEST_F(EdateEomonthTest, EDATE_InvalidArgs) {
    auto result1 = edate({}, context);
    EXPECT_TRUE(result1.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result1.asError());

    auto start = date({Value(2024.0), Value(1.0), Value(1.0)}, context);
    auto result2 = edate({start}, context);
    EXPECT_TRUE(result2.isError());

    auto result3 = edate({Value(123.0), Value(1.0)}, context);
    EXPECT_TRUE(result3.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result3.asError());
}

TEST_F(EdateEomonthTest, EOMONTH_Basic) {
    auto start = date({Value(2023.0), Value(1.0), Value(15.0)}, context);
    ASSERT_TRUE(start.isDate());

    auto result = eomonth({start, Value(0.0)}, context);
    ASSERT_TRUE(result.isDate());

    auto tp = result.asDate();
    auto tt = std::chrono::system_clock::to_time_t(tp);
    auto tm = *std::localtime(&tt);
    EXPECT_EQ(2023, tm.tm_year + 1900);
    EXPECT_EQ(1, tm.tm_mon + 1);
    EXPECT_EQ(31, tm.tm_mday);
}

TEST_F(EdateEomonthTest, EOMONTH_NextMonth) {
    auto start = date({Value(2023.0), Value(1.0), Value(15.0)}, context);
    auto result = eomonth({start, Value(1.0)}, context);
    ASSERT_TRUE(result.isDate());

    auto tp = result.asDate();
    auto tt = std::chrono::system_clock::to_time_t(tp);
    auto tm = *std::localtime(&tt);
    EXPECT_EQ(2023, tm.tm_year + 1900);
    EXPECT_EQ(2, tm.tm_mon + 1);
    EXPECT_EQ(28, tm.tm_mday);
}

TEST_F(EdateEomonthTest, EOMONTH_InvalidArgs) {
    auto result1 = eomonth({}, context);
    EXPECT_TRUE(result1.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result1.asError());
}