#include <chrono>
#include <ctime>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief MONTH function - returns the month from a date
 * @param args Function arguments (expects 1 date argument)
 * @param context Evaluation context (unused for MONTH)
 * @return Month as numeric value (1-12)
 */
Value month(const std::vector<Value>& args, const Context& context) {
    return templates::singleDateFunction(args, context, "MONTH", [](const std::tm& tm) {
        return tm.tm_mon + 1;
    });  // tm_mon is 0-based
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula