#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief DAY function - returns the day from a date
 * @param args Function arguments (expects 1 date argument)
 * @param context Evaluation context (unused for DAY)
 * @return Day as numeric value (1-31)
 */
Value day(const std::vector<Value>& args, const Context& context) {
    return templates::singleDateFunction(args, context, "DAY",
                                         [](const std::tm& tm) { return tm.tm_mday; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula