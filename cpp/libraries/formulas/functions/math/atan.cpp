#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the arctangent of a number
 * @ingroup math
 * @param number Value whose arctangent is returned
 * @code
 * ATAN(1) -> PI()/4
 * @endcode
 */
Value atan_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "ATAN",
                                            [](double x) { return std::atan(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula