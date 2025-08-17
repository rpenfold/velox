#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the cosine of an angle (in radians)
 * @ingroup math
 * @param angle Angle in radians
 * @code
 * COS(0) -> 1
 * @endcode
 */
Value cos_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "COS",
                                            [](double x) { return std::cos(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula