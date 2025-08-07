#include "xl-formula/functions.h"
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief MROUND function - rounds a number to the nearest multiple
 * @param args Function arguments (number, multiple)
 * @param context Evaluation context (unused for MROUND)
 * @return Number rounded to the nearest multiple
 * 
 * Excel syntax: MROUND(number, multiple)
 * - number: The value to round
 * - multiple: The multiple to which to round
 * 
 * The function:
 * - Rounds number to the nearest multiple of multiple
 * - If number and multiple have different signs, returns #NUM! error
 * - If multiple is 0, returns 0
 * - Examples: MROUND(10, 3) = 9, MROUND(1.3, 0.2) = 1.4, MROUND(-10, -3) = -9
 */
Value mround(const std::vector<Value>& args, const Context& context) {
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
    auto numberResult = utils::toNumberSafe(args[0], "MROUND");
    if (numberResult.isError()) {
        return numberResult;
    }
    double number = numberResult.asNumber();

    auto multipleResult = utils::toNumberSafe(args[1], "MROUND");
    if (multipleResult.isError()) {
        return multipleResult;
    }
    double multiple = multipleResult.asNumber();

    // Handle special case: multiple is 0
    if (multiple == 0.0) {
        return Value(0.0);
    }

    // Check if number and multiple have different signs
    if ((number > 0 && multiple < 0) || (number < 0 && multiple > 0)) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    // Handle zero number case
    if (number == 0.0) {
        return Value(0.0);
    }

    // Round to nearest multiple
    // Formula: ROUND(number / multiple, 0) * multiple
    double ratio = number / multiple;
    double rounded_ratio = std::round(ratio);
    double result = rounded_ratio * multiple;

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
