#include <chrono>
#include <cmath>
#include <ctime>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief HOUR function - returns the hour from a time/date
 * @param args Function arguments (expects 1 date/time or numeric argument)
 * @param context Evaluation context (unused for HOUR)
 * @return Hour as numeric value (0-23)
 */
Value hour(const std::vector<Value>& args, const Context& context) {
    return templates::dateTimeExtractionFunction(
            args, context, "HOUR", [](const std::tm& tm) { return tm.tm_hour; },
            [](double time_fraction) {
                return datetime_utils::extractHourFromFraction(time_fraction);
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula