#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;

TEST(NonStdFunctionsTest, UnixTimeFromNow_IsClose) {
    FormulaEngine engine;
    auto nowRes = engine.evaluate("NOW()");
    ASSERT_TRUE(nowRes.isSuccess());
    engine.getContext();
    engine.setVariable("_NOW", nowRes.getValue());
    auto res = engine.evaluate("NS_UNIXTIME(_NOW)");
    ASSERT_TRUE(res.isSuccess());
    auto secs = res.getValue().asNumber();
    // Should be within a day range of plausible value (> 1e9 somewhere)
    ASSERT_GT(secs, 1000000000.0);
}

TEST(NonStdFunctionsTest, NearestDateFromArray) {
    FormulaEngine engine;
    auto nowRes = engine.evaluate("NOW()");
    ASSERT_TRUE(nowRes.isSuccess());
    Value nowVal = nowRes.getValue();

    // Build array: now-1d, now, now+2d
    auto now_tp = nowVal.asDate();
    auto day = std::chrono::hours(24);
    std::vector<Value> arr = { Value(now_tp - day), Value(now_tp), Value(now_tp + day + day) };
    engine.setVariable("ARR", Value(arr));
    auto res = engine.evaluate("NS_NEARESTDATE(ARR)");
    ASSERT_TRUE(res.isSuccess());
    auto v = res.getValue();
    ASSERT_TRUE(v.isDate());
    // Expect exact now
    ASSERT_EQ(std::chrono::duration_cast<std::chrono::seconds>(v.asDate().time_since_epoch()).count(),
              std::chrono::duration_cast<std::chrono::seconds>(now_tp.time_since_epoch()).count());
}

TEST(NonStdFunctionsTest, FurthestDateFromArray) {
    FormulaEngine engine;
    auto nowRes = engine.evaluate("NOW()");
    ASSERT_TRUE(nowRes.isSuccess());
    Value nowVal = nowRes.getValue();
    auto now_tp = nowVal.asDate();
    auto day = std::chrono::hours(24);
    std::vector<Value> arr = { Value(now_tp - day * 1), Value(now_tp + day * 3) };
    engine.setVariable("ARR", Value(arr));
    auto res = engine.evaluate("NS_FURTHESTDATE(ARR)");
    ASSERT_TRUE(res.isSuccess());
    auto v = res.getValue();
    ASSERT_TRUE(v.isDate());
    // Furthest is +3d
    ASSERT_EQ(std::chrono::duration_cast<std::chrono::seconds>(v.asDate().time_since_epoch()).count(),
              std::chrono::duration_cast<std::chrono::seconds>((now_tp + day * 3).time_since_epoch()).count());
}


