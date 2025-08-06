#include <algorithm>
#include <unordered_map>
#include <vector>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief MODE function - returns the most frequently occurring value
 * @param args Function arguments (expects 1+ numeric arguments)
 * @param context Evaluation context (unused for MODE)
 * @return Most frequently occurring value
 */
Value mode(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // MODE requires at least one argument
    auto error = utils::validateMinArgs(args, 1, "MODE");
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
        // Non-numeric values are ignored in MODE (Excel behavior)
    }

    // If no numeric values found, return error
    if (numbers.empty()) {
        return Value::error(ErrorType::DIV_ZERO);
    }

    // Count frequency of each number
    std::unordered_map<double, int> frequency;
    for (double num : numbers) {
        frequency[num]++;
    }

    // Find the number with highest frequency
    double mode_value = numbers[0];
    int max_frequency = 1;

    for (const auto& pair : frequency) {
        if (pair.second > max_frequency) {
            max_frequency = pair.second;
            mode_value = pair.first;
        }
    }

    // If multiple numbers have the same frequency, return the first one in original order
    if (max_frequency > 1) {
        for (double num : numbers) {
            if (frequency[num] == max_frequency) {
                mode_value = num;
                break;
            }
        }
    }

    // If all numbers appear only once, return the first number (Excel behavior)
    if (max_frequency == 1) {
        return Value(numbers[0]);
    }

    return Value(mode_value);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula