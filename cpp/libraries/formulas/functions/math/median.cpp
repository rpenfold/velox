#include <algorithm>
#include <vector>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the median (middle value) of the arguments
 * @ingroup math
 * @param number1 First number
 * @param number2 Additional numbers (optional, variadic)
 * @code
 * MEDIAN(1, 2, 3, 4) -> 2.5
 * @endcode
 */
Value median(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // MEDIAN requires at least one argument
    auto error = utils::validateMinArgs(args, 1, "MEDIAN");
    if (!error.isEmpty()) {
        return error;
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::vector<double> numbers;

    // Collect all numeric values
    for (const auto& arg : args) {
        if (arg.isEmpty()) {
            continue;  // Skip empty values
        }

        if (arg.canConvertToNumber()) {
            numbers.push_back(arg.toNumber());
        }
        // Non-numeric values are ignored in MEDIAN (Excel behavior)
    }

    // If no numeric values found, return error
    if (numbers.empty()) {
        return Value::error(ErrorType::DIV_ZERO);
    }

    // Sort the numbers
    std::sort(numbers.begin(), numbers.end());

    // Calculate median
    size_t size = numbers.size();
    if (size % 2 == 0) {
        // Even number of elements - average of two middle values
        double mid1 = numbers[size / 2 - 1];
        double mid2 = numbers[size / 2];
        return Value((mid1 + mid2) / 2.0);
    } else {
        // Odd number of elements - middle value
        return Value(numbers[size / 2]);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula