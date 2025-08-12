#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Rounds a number down to the nearest multiple of significance
 * @ingroup math
 * @param number The number to round down
 * @param significance The multiple to round down to (optional)
 * @code
 * FLOOR(4.7) -> 4
 * @endcode
 */
Value floor_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // Validate argument count (1 or 2 arguments)
    if (args.empty() || args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }

    // Convert first argument to number
    if (!args[0].canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    double value = args[0].toNumber();

    // If only one argument, round down to nearest integer
    if (args.size() == 1) {
        return Value(std::floor(value));
    }

    // If two arguments, second is the significance (multiple)
    if (!args[1].canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    double sig = args[1].toNumber();

    // Handle zero significance
    if (sig == 0.0) {
        return Value::error(ErrorType::DIV_ZERO);
    }

    // For negative significance, both number and significance must have same sign
    if (sig < 0.0 && value > 0.0) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    // Calculate floor to nearest multiple
    if (value == 0.0) {
        return Value(0.0);
    }

    if (sig < 0.0) {
        // Both negative
        return Value(std::ceil(value / sig) * sig);
    } else {
        // Positive significance
        return Value(std::floor(value / sig) * sig);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula