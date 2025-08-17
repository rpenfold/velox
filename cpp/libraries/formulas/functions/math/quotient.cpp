#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the integer portion of a division
 * @ingroup math
 * @param numerator Dividend
 * @param denominator Divisor
 * @code
 * QUOTIENT(10, 3) -> 3
 * @endcode
 */
Value quotient(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // Validate argument count
    if (args.size() != 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert arguments to numbers
    auto numeratorResult = utils::toNumberSafe(args[0], "QUOTIENT");
    if (numeratorResult.isError()) {
        return numeratorResult;
    }
    double numerator = numeratorResult.asNumber();

    auto denominatorResult = utils::toNumberSafe(args[1], "QUOTIENT");
    if (denominatorResult.isError()) {
        return denominatorResult;
    }
    double denominator = denominatorResult.asNumber();

    // Check for division by zero
    if (denominator == 0.0) {
        return Value::error(ErrorType::DIV_ZERO);
    }

    // Perform division and truncate toward zero
    double result = numerator / denominator;

    // Use trunc() to truncate toward zero (removes fractional part)
    // This is different from floor() which always rounds down
    double quotient_result = std::trunc(result);

    return Value(quotient_result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
