#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the sine of an angle (in radians)
 * @ingroup math
 * @param angle Angle in radians
 * @code
 * SIN(PI()/2) -> 1
 * @endcode
 */
Value sin_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "SIN",
                                            [](double x) { return std::sin(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula