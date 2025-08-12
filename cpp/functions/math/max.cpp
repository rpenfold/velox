#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the largest value among the arguments
 * @ingroup math
 * @param number1 First number (optional)
 * @param number2 Additional numbers (variadic)
 * @code
 * MAX(10,20,5) -> 20
 * @endcode
 */
Value max(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateMinArgs(args, 1, "MAX");
    if (!validation.isEmpty()) {
        return validation;
    }

    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }

    Value maxValue;
    bool hasValue = false;

    for (const auto& arg : args) {
        if (arg.isEmpty())
            continue;

        if (!hasValue) {
            maxValue = arg;
            hasValue = true;
        } else {
            if (arg > maxValue) {
                maxValue = arg;
            }
        }
    }

    if (!hasValue) {
        return Value(0.0);  // Excel returns 0 for MAX with no valid values
    }

    return maxValue;
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula