#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief SUMSQ function - returns the sum of the squares of the arguments
 * @param args Function arguments (number values)
 * @param context Evaluation context (unused for SUMSQ)
 * @return Sum of squares of all numeric arguments
 *
 * Excel syntax: SUMSQ(number1, [number2], ...)
 * - number1, number2, ...: Numbers to square and sum
 *
 * The function:
 * - Accepts any number of arguments (at least 1 required)
 * - Squares each numeric value and sums them
 * - Ignores text, logical values, and empty cells
 * - Returns 0 if no numeric arguments are provided
 */
Value sumsq(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // Validate minimum argument count
    auto minValidation = utils::validateMinArgs(args, 1, "SUMSQ");
    if (!minValidation.isEmpty()) {
        return minValidation;
    }

    // Check for errors in arguments and propagate the first one found
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    double sum = 0.0;

    for (const Value& arg : args) {
        if (arg.isNumber()) {
            double num = arg.asNumber();
            sum += num * num;  // Square the number and add to sum
        }
        // Ignore non-numeric values (text, boolean, empty)
        // This matches Excel's behavior for SUMSQ
    }

    return Value(sum);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
