#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the hyperbolic sine of a number
 * @ingroup math
 * @param number Value whose hyperbolic sine is returned
 * @code
 * SINH(0) -> 0
 * @endcode
 */
Value sinh_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "SINH",
                                            [](double x) { return std::sinh(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula