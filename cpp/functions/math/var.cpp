#include <cmath>
#include <vector>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief VAR function - returns the variance of a set of numbers
 * @param args Function arguments (expects 1+ numeric arguments)
 * @param context Evaluation context (unused for VAR)
 * @return Variance of the arguments
 */
Value var(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // VAR requires at least one argument
    auto error = utils::validateMinArgs(args, 1, "VAR");
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
        // Non-numeric values are ignored in VAR (Excel behavior)
    }

    // If no numeric values found, return error
    if (numbers.empty()) {
        return Value::error(ErrorType::DIV_ZERO);
    }

    // If only one value, return error (Excel behavior)
    if (numbers.size() == 1) {
        return Value::error(ErrorType::DIV_ZERO);
    }

    // Calculate mean
    double sum = 0.0;
    for (double num : numbers) {
        sum += num;
    }
    double mean = sum / numbers.size();

    // Calculate sum of squared differences
    double sum_squared_diff = 0.0;
    for (double num : numbers) {
        double diff = num - mean;
        sum_squared_diff += diff * diff;
    }

    // Calculate variance (sample variance)
    double variance = sum_squared_diff / (numbers.size() - 1);

    return Value(variance);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula