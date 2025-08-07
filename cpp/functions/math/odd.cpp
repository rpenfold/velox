#include "xl-formula/functions.h"
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief ODD function - rounds a number up to the nearest odd integer
 * @param args Function arguments (number)
 * @param context Evaluation context (unused for ODD)
 * @return Nearest odd integer >= number (for positive) or <= number (for negative)
 * 
 * Excel syntax: ODD(number)
 * - number: The value to round to the nearest odd integer
 * 
 * The function:
 * - For positive numbers: rounds up to the next odd integer
 * - For negative numbers: rounds away from zero to the next odd integer
 * - Examples: ODD(1.2) = 3, ODD(2) = 3, ODD(-1) = -1, ODD(-2.5) = -3
 */
Value odd_function(const std::vector<Value>& args, const Context& context) {
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
    auto numberResult = utils::toNumberSafe(args[0], "ODD");
    if (numberResult.isError()) {
        return numberResult;
    }
    double number = numberResult.asNumber();

    // Handle zero case (rounds to 1)
    if (number == 0.0) {
        return Value(1.0);
    }

    double result;
    if (number > 0) {
        // For positive numbers, round up to next odd integer
        result = std::ceil(number);
        // If result is even, add 1 to make it odd
        if (std::fmod(result, 2.0) == 0.0) {
            result += 1.0;
        }
    } else {
        // For negative numbers, round away from zero to next odd integer
        result = std::floor(number);
        // If result is even, subtract 1 to make it odd (more negative)
        if (std::fmod(std::abs(result), 2.0) == 0.0) {
            result -= 1.0;
        }
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
