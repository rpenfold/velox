#include <chrono>
#include <cmath>
#include <limits>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

// Helper to iterate date candidates from args. Supports either a single array arg or variadic args.
static void forEachDateCandidate(const std::vector<Value>& args, const std::function<void(const Value&)>& fn) {
    if (args.size() == 1 && args[0].isArray()) {
        for (const auto& v : args[0].asArray()) {
            fn(v);
        }
        return;
    }
    for (const auto& v : args) {
        fn(v);
    }
}

/**
 * @brief [Non-Standard] Returns Unix epoch seconds for a given date
 * @ingroup nonstandard
 * @name NS_UNIXTIME
 * @param date Date/time value to convert
 * @code
 * NS_UNIXTIME(NOW()) -> 1697040000
 * @endcode
 */
Value ns_unixtime(const std::vector<Value>& args, const Context& context) {
    (void)context;
    // Expect exactly one argument
    auto check = utils::validateArgCount(args, 1, "NS_UNIXTIME");
    if (check.isError()) return check;
    const Value& v = args[0];
    if (v.isError()) return v;
    if (!v.isDate()) return Value::error(ErrorType::VALUE_ERROR);
    using namespace std::chrono;
    auto tp = v.asDate();
    auto secs = duration_cast<seconds>(tp.time_since_epoch()).count();
    return Value(static_cast<double>(secs));
}

/**
 * @brief [Non-Standard] Returns the date closest to the current time from a list/array
 * @ingroup nonstandard
 * @name NS_NEARESTDATE
 * @param dates Array of dates or variadic list of date arguments (…)
 * @code
 * NS_NEARESTDATE({TODAY()-1, TODAY(), TODAY()+2}) -> 2025-01-01
 * NS_NEARESTDATE(TODAY()-1, TODAY()+3) -> 2024-12-31
 * @endcode
 */
Value ns_nearestdate(const std::vector<Value>& args, const Context& context) {
    (void)context;
    using namespace std::chrono;
    const auto now = system_clock::now();
    bool found = false;
    system_clock::time_point best_tp{};
    auto best_dist = std::numeric_limits<long double>::infinity();

    forEachDateCandidate(args, [&](const Value& v) {
        if (v.isError()) return;  // skip
        if (!v.isDate()) return;
        auto tp = v.asDate();
        auto dist = std::chrono::duration<long double>(tp - now);
        long double ad = std::fabsl(dist.count());
        if (!found || ad < best_dist) {
            found = true;
            best_dist = ad;
            best_tp = tp;
        }
    });

    if (!found) return Value::error(ErrorType::NA_ERROR);
    return Value(best_tp);
}

/**
 * @brief [Non-Standard] Returns the date farthest from the current time from a list/array
 * @ingroup nonstandard
 * @name NS_FURTHESTDATE
 * @param dates Array of dates or variadic list of date arguments (…)
 * @code
 * NS_FURTHESTDATE({TODAY()-1, TODAY()+3}) -> 2025-01-03
 * NS_FURTHESTDATE(TODAY()-10, TODAY()+2, TODAY()+5) -> 2024-12-22
 * @endcode
 */
Value ns_furthestdate(const std::vector<Value>& args, const Context& context) {
    (void)context;
    using namespace std::chrono;
    const auto now = system_clock::now();
    bool found = false;
    system_clock::time_point best_tp{};
    long double best_dist = -1.0L;

    forEachDateCandidate(args, [&](const Value& v) {
        if (v.isError()) return;  // skip
        if (!v.isDate()) return;
        auto tp = v.asDate();
        auto dist = std::chrono::duration<long double>(tp - now);
        long double ad = std::fabsl(dist.count());
        if (!found || ad > best_dist) {
            found = true;
            best_dist = ad;
            best_tp = tp;
        }
    });

    if (!found) return Value::error(ErrorType::NA_ERROR);
    return Value(best_tp);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


