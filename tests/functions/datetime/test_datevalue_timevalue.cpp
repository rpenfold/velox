#include <gtest/gtest.h>
#include <chrono>
#include <ctime>
#include "velox/formulas/functions.h"

using namespace xl_formula::functions::builtin;
using xl_formula::Context;
using xl_formula::Value;

TEST(DateParseFunctionsTest, DateValue_ParsesIso) {
    auto v = datevalue({Value("2024-01-15")}, Context{});
    ASSERT_TRUE(v.isDate());
}

TEST(DateParseFunctionsTest, DateValue_ParsesSlashAndUSOrder) {
    auto v1 = datevalue({Value("2024/02/05")}, Context{});
    ASSERT_TRUE(v1.isDate());
    auto v2 = datevalue({Value("2/5/2024")}, Context{});
    ASSERT_TRUE(v2.isDate());
}

TEST(DateParseFunctionsTest, TimeValue_ParsesColon) {
    auto v = timevalue({Value("12:00:00")}, Context{});
    ASSERT_TRUE(v.isNumber());
    EXPECT_NEAR(v.asNumber(), 0.5, 1e-9);
}

TEST(DateParseFunctionsTest, TimeValue_ParsesAMPM) {
    auto v1 = timevalue({Value("12:00 AM")}, Context{});
    ASSERT_TRUE(v1.isNumber());
    EXPECT_NEAR(v1.asNumber(), 0.0, 1e-9);
    auto v2 = timevalue({Value("12:00 PM")}, Context{});
    ASSERT_TRUE(v2.isNumber());
    EXPECT_NEAR(v2.asNumber(), 0.5, 1e-9);
    auto v3 = timevalue({Value("1:30 pm")}, Context{});
    ASSERT_TRUE(v3.isNumber());
    EXPECT_NEAR(v3.asNumber(), (13 * 3600.0 + 30 * 60.0) / (24.0 * 3600.0), 1e-9);
}
