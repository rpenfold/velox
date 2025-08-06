#include <chrono>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief TODAY function - returns the current date (without time)
 * @param args Function arguments (expects 0 arguments)
 * @param context Evaluation context (unused for TODAY)
 * @return Current date at midnight as Value
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