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
 * @brief [Non-Standard] NS_UNIXTIME - returns Unix epoch seconds for a given date
 * @param args (date)
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
 * @brief [Non-Standard] NS_NEARESTDATE - returns the date closest to current time from a list/array
 * @param args ([dates] or variadic dates)
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
 * @brief [Non-Standard] NS_FURTHESTDATE - returns the date farthest from current time from a list/array
 * @param args ([dates] or variadic dates)
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


