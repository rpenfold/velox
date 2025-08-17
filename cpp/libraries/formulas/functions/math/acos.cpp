#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the arccosine of a number
 * @ingroup math
 * @param number Value in range [-1, 1]
 * @code
 * ACOS(1) -> 0
 * @endcode
 */
Value acos_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "ACOS", [](double x) {
        if (x < -1.0 || x > 1.0) {
            throw std::runtime_error("Domain error");
        }
        return std::acos(x);
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula