#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Rounds a number up to the nearest even integer
 * @ingroup math
 * @param number The value to round
 * @code
 * EVEN(3) -> 4
 * @endcode
 */
Value even_function(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // Validate argument count
    if (args.size() != 1) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert argument to number
    auto numberResult = utils::toNumberSafe(args[0], "EVEN");
    if (numberResult.isError()) {
        return numberResult;
    }
    double number = numberResult.asNumber();

    // Handle zero case
    if (number == 0.0) {
        return Value(0.0);
    }

    double result;
    if (number > 0) {
        // For positive numbers, round up to next even integer
        result = std::ceil(number);
        // If result is odd, add 1 to make it even
        if (std::fmod(result, 2.0) != 0.0) {
            result += 1.0;
        }
    } else {
        // For negative numbers, round away from zero to next even integer
        result = std::floor(number);
        // If result is odd, subtract 1 to make it even (more negative)
        if (std::fmod(std::abs(result), 2.0) != 0.0) {
            result -= 1.0;
        }
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
