#include "xl-formula/functions.h"
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Rounds a number to the nearest multiple
 * @ingroup math
 * @param number The value to round
 * @param multiple The multiple to which to round
 * @code
 * MROUND(10, 3) -> 9
 * @endcode
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
