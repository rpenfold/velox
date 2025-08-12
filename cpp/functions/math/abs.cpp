#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the absolute value of a number
 * @ingroup math
 * @param number Number to convert to absolute value
 * @code
 * ABS(-5) -> 5
 * @endcode
 */
Value abs_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "ABS",
                                            [](double x) { return std::abs(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula