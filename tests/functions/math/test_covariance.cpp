#include <gtest/gtest.h>
#include "velox/formulas/functions.h"

using namespace xl_formula::functions::builtin;
using xl_formula::Context;
using xl_formula::Value;

TEST(CovarianceFunctionTest, PopulationAndSample) {
    std::vector<Value> x = {Value(1.0), Value(2.0), Value(3.0)};
    std::vector<Value> y = {Value(2.0), Value(4.0), Value(6.0)};
    auto covp = covariance_p({Value(x), Value(y)}, Context{});
    ASSERT_TRUE(covp.isNumber());
    EXPECT_NEAR(covp.asNumber(), 4.0 / 3.0, 1e-9);
    auto covs = covariance_s({Value(x), Value(y)}, Context{});
    ASSERT_TRUE(covs.isNumber());
    EXPECT_NEAR(covs.asNumber(), 2.0, 1e-9);
    auto covar_hist = covar({Value(x), Value(y)}, Context{});
    ASSERT_TRUE(covar_hist.isNumber());
    EXPECT_NEAR(covar_hist.asNumber(), 4.0 / 3.0, 1e-9);
}
