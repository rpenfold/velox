#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief TIME function - creates a time from hour, minute, second
 * @param args Function arguments (expects 3 numeric arguments: hour, minute, second)
 * @param context Evaluation context (unused for TIME)
 * @return Time value as a fraction of a day (Excel-style)
 */
Value time_function(const std::vector<Value>& args, const Context& context) {
    return templates::threeNumberFunction(
            args, context, "TIME", [](int hour, int minute, int second) -> Value {
                // Validate ranges
                if (hour < 0 || hour > 23) {
                    throw std::runtime_error("Hour out of range");
                }
                if (minute < 0 || minute > 59) {
                    throw std::runtime_error("Minute out of range");
                }
                if (second < 0 || second > 59) {
                    throw std::runtime_error("Second out of range");
                }

                // Calculate time as fraction of a day (Excel convention)
                double total_seconds = hour * 3600.0 + minute * 60.0 + second;
                double seconds_per_day = 24.0 * 60.0 * 60.0;
                double time_fraction = total_seconds / seconds_per_day;

                return Value(time_fraction);
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula