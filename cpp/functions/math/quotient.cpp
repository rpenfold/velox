#include "xl-formula/functions.h"
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief QUOTIENT function - returns the integer portion of a division
 * @param args Function arguments (numerator, denominator)
 * @param context Evaluation context (unused for QUOTIENT)
 * @return Integer portion of numerator divided by denominator
 * 
 * Excel syntax: QUOTIENT(numerator, denominator)
 * - numerator: The dividend (number to be divided)
 * - denominator: The divisor (number to divide by)
 * 
 * The function:
 * - Returns the integer portion of numerator/denominator
 * - Truncates toward zero (same as INT function on division result)
 * - Returns #DIV/0! error if denominator is zero
 * - Both arguments must be numeric
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
