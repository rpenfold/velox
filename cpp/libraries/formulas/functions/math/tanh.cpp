#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the hyperbolic tangent of a number
 * @ingroup math
 * @param number Value whose hyperbolic tangent is returned
 * @code
 * TANH(0) -> 0
 * @endcode
 */
Value tanh_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "TANH",
                                            [](double x) { return std::tanh(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula