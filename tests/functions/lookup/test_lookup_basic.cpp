#include <gtest/gtest.h>
#include "velox/formulas/functions.h"

using namespace xl_formula::functions::builtin;
using xl_formula::Context;
using xl_formula::Value;

TEST(LookupBasicTest, Choose_Works) {
    auto r = choose({Value(2.0), Value("A"), Value("B"), Value("C")}, Context{});
    ASSERT_TRUE(r.isText());
    EXPECT_EQ(r.asText(), "B");
}

TEST(LookupBasicTest, RowColumn_Stubs) {
    auto row = row_function({}, Context{});
    ASSERT_TRUE(row.isNumber());
    EXPECT_DOUBLE_EQ(row.asNumber(), 1.0);
    auto col = column_function({}, Context{});
    ASSERT_TRUE(col.isNumber());
    EXPECT_DOUBLE_EQ(col.asNumber(), 1.0);
}


