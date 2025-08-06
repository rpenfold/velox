#include <chrono>
#include <cmath>
#include <ctime>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief MINUTE function - returns the minute from a time/date
 * @param args Function arguments (expects 1 date/time or numeric argument)
 * @param context Evaluation context (unused for MINUTE)
 * @return Minute as numeric value (0-59)
 */
Value minute(const std::vector<Value>& args, const Context& context) {
    return templates::dateTimeExtractionFunction(
            args, context, "MINUTE", [](const std::tm& tm) { return tm.tm_min; },
            [](double time_fraction) {
                return datetime_utils::extractMinuteFromFraction(time_fraction);
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula