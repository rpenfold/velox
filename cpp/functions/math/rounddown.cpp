#include "xl-formula/functions.h"
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief ROUNDDOWN function - rounds a number down, toward zero
 * @param args Function arguments (number, num_digits)
 * @param context Evaluation context (unused for ROUNDDOWN)
 * @return Number rounded down to specified decimal places
 * 
 * Excel syntax: ROUNDDOWN(number, num_digits)
 * - number: The number to round down
 * - num_digits: The number of digits to which to round
 * 
 * The function:
 * - Always rounds toward zero
 * - Positive num_digits: rounds to decimal places
 * - Zero num_digits: rounds to nearest integer
 * - Negative num_digits: rounds to left of decimal point
 * - Examples: ROUNDDOWN(3.9, 0) = 3, ROUNDDOWN(-3.9, 0) = -3, ROUNDDOWN(38.9, -1) = 30
 */
Value rounddown(const std::vector<Value>& args, const Context& context) {
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
    auto numberResult = utils::toNumberSafe(args[0], "ROUNDDOWN");
    if (numberResult.isError()) {
        return numberResult;
    }
    double number = numberResult.asNumber();

    auto digitsResult = utils::toNumberSafe(args[1], "ROUNDDOWN");
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
        // For positive numbers, use floor to round toward zero
        result = std::floor(number * factor) / factor;
    } else {
        // For negative numbers, use ceiling to round toward zero (less negative)
        result = std::ceil(number * factor) / factor;
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
