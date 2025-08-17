#include <chrono>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the current date (without time)
 * @ingroup datetime
 * @code
 * TODAY() -> 2025-08-11 00:00:00
 * @endcode
 */
Value today(const std::vector<Value>& args, const Context& context) {
    return templates::noArgFunction(args, context, "TODAY", []() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto local_tm = *std::localtime(&time_t);

        // Set time to midnight
        local_tm.tm_hour = 0;
        local_tm.tm_min = 0;
        local_tm.tm_sec = 0;

        // Convert back to time_point
        auto midnight = std::chrono::system_clock::from_time_t(std::mktime(&local_tm));
        return Value(midnight);
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula