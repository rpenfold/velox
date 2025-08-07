#include "xl-formula/functions.h"
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief ROUNDUP function - rounds a number up, away from zero
 * @param args Function arguments (number, num_digits)
 * @param context Evaluation context (unused for ROUNDUP)
 * @return Number rounded up to specified decimal places
 * 
 * Excel syntax: ROUNDUP(number, num_digits)
 * - number: The number to round up
 * - num_digits: The number of digits to which to round
 * 
 * The function:
 * - Always rounds away from zero
 * - Positive num_digits: rounds to decimal places
 * - Zero num_digits: rounds to nearest integer
 * - Negative num_digits: rounds to left of decimal point
 * - Examples: ROUNDUP(3.2, 0) = 4, ROUNDUP(-3.2, 0) = -4, ROUNDUP(31.9, -1) = 40
 */
Value roundup(const std::vector<Value>& args, const Context& context) {
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
    auto numberResult = utils::toNumberSafe(args[0], "ROUNDUP");
    if (numberResult.isError()) {
        return numberResult;
    }
    double number = numberResult.asNumber();

    auto digitsResult = utils::toNumberSafe(args[1], "ROUNDUP");
    if (digitsResult.isError()) {
        return digitsResult;
    }
    double num_digits = digitsResult.asNumber();

    // Handle zero case
    if (number == 0.0) {
        return Value(0.0);
    }

    // Calculate the factor for rounding
    double factor = std::pow(10.0, num_digits);
    
    double result;
    if (number > 0) {
        // For positive numbers, use ceiling to round away from zero
        result = std::ceil(number * factor) / factor;
    } else {
        // For negative numbers, use floor to round away from zero (more negative)
        result = std::floor(number * factor) / factor;
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
