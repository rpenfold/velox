#include "xl-formula/functions.h"
#include <chrono>
#include <ctime>
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief SECOND function - returns the second from a time/date
 * @param args Function arguments (expects 1 date/time or numeric argument)
 * @param context Evaluation context (unused for SECOND)
 * @return Second as numeric value (0-59)
 */
Value second(const std::vector<Value>& args, const Context& context) {
    return templates::dateTimeExtractionFunction(args, context, "SECOND",
        [](const std::tm& tm) { return tm.tm_sec; },
        [](double time_fraction) { return datetime_utils::extractSecondFromFraction(time_fraction); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula