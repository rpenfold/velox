#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the natural logarithm of a number
 * @ingroup math
 * @param number Positive number
 * @code
 * LN(EXP(1)) -> 1
 * @endcode
 */
Value ln_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "LN", [](double x) {
        if (x <= 0.0) {
            throw std::runtime_error("Domain error");
        }
        return std::log(x);
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula