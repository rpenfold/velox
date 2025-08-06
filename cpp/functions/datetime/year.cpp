#include "xl-formula/functions.h"
#include <chrono>
#include <ctime>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief YEAR function - returns the year from a date
 * @param args Function arguments (expects 1 date argument)
 * @param context Evaluation context (unused for YEAR)
 * @return Year as numeric value
 */
Value year(const std::vector<Value>& args, const Context& context) {
    return templates::singleDateFunction(args, context, "YEAR",
        [](const std::tm& tm) { return tm.tm_year + 1900; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula