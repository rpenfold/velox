#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the hyperbolic cosine of a number
 * @ingroup math
 * @param number Value whose hyperbolic cosine is returned
 * @code
 * COSH(0) -> 1
 * @endcode
 */
Value cosh_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "COSH",
                                            [](double x) { return std::cosh(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula