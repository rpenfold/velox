#include <xl-formula/functions.h>
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the square root of a number
 * @ingroup math
 * @param number The number to get square root of
 * @code
 * SQRT(16) -> 4
 * @endcode
 */
Value sqrt_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "SQRT", [](double x) {
        if (x < 0.0) {
            throw std::runtime_error("Negative number");  // Will be caught and converted to error
        }
        return std::sqrt(x);
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula