#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Rounds a number down to the nearest integer
 * @ingroup math
 * @param number The number to round down
 * @code
 * INT(3.9) -> 3
 * @endcode
 */
Value int_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "INT",
                                            [](double x) { return std::floor(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula