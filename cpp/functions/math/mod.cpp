#include <xl-formula/functions.h>
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the remainder after division
 * @ingroup math
 * @param number The number to divide (numerator)
 * @param divisor The divisor
 * @code
 * MOD(10, 3) -> 1
 * @endcode
 */
Value mod(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // MOD requires exactly two arguments
    auto error = utils::validateArgCount(args, 2, "MOD");
    if (!error.isEmpty()) {
        return error;
    }

    // Convert both arguments to numbers safely
    auto numeratorValue = utils::toNumberSafe(args[0], "MOD");
    if (numeratorValue.isError()) {
        return numeratorValue;
    }

    auto divisorValue = utils::toNumberSafe(args[1], "MOD");
    if (divisorValue.isError()) {
        return divisorValue;
    }

    double numerator = numeratorValue.asNumber();
    double divisor = divisorValue.asNumber();

    // Division by zero returns #DIV/0! error
    if (divisor == 0.0) {
        return Value::error(ErrorType::DIV_ZERO);
    }

    // Use fmod for floating point modulo
    double result = std::fmod(numerator, divisor);

    // Excel MOD function always returns a result with the same sign as the divisor
    if ((result > 0.0 && divisor < 0.0) || (result < 0.0 && divisor > 0.0)) {
        result += divisor;
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula