#include <velox/formulas/functions.h>
#include <cmath>
#include <limits>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns a number raised to a power
 * @ingroup math
 * @param number The base number
 * @param power The exponent
 * @code
 * POWER(2, 3) -> 8
 * @endcode
 */
Value power(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // POWER requires exactly two arguments
    auto error = utils::validateArgCount(args, 2, "POWER");
    if (!error.isEmpty()) {
        return error;
    }

    // Convert both arguments to numbers safely
    auto baseValue = utils::toNumberSafe(args[0], "POWER");
    if (baseValue.isError()) {
        return baseValue;
    }

    auto exponentValue = utils::toNumberSafe(args[1], "POWER");
    if (exponentValue.isError()) {
        return exponentValue;
    }

    double base = baseValue.asNumber();
    double exponent = exponentValue.asNumber();

    // Handle special cases
    if (base == 0.0 && exponent < 0.0) {
        return Value::error(ErrorType::DIV_ZERO);
    }

    if (base < 0.0 && std::floor(exponent) != exponent) {
        // Negative base with non-integer exponent returns #NUM! error
        return Value::error(ErrorType::NUM_ERROR);
    }

    double result = std::pow(base, exponent);

    // Check for overflow/underflow
    if (!std::isfinite(result)) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula