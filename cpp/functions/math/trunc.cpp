#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief TRUNC function - truncates a number to an integer
 * @param args Function arguments (expects 1 or 2 arguments)
 * @param context Evaluation context (unused for TRUNC)
 * @return Truncated number
 */
Value trunc_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
        // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }
    
    // Validate argument count (1 or 2 arguments)
    if (args.empty() || args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    // Convert first argument to number
    if (!args[0].canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    double value = args[0].toNumber();

    // If only one argument, truncate to integer
    if (args.size() == 1) {
        return Value(std::trunc(value));
    }

        // If two arguments, second is number of decimal places
    if (!args[1].canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    int num_digits = static_cast<int>(args[1].toNumber());

    // Handle negative digits (truncate to tens, hundreds, etc.)
    if (num_digits < 0) {
        double factor = std::pow(10.0, -num_digits);
        return Value(std::trunc(value / factor) * factor);
    }

    // Handle positive digits (truncate to decimal places)
    double factor = std::pow(10.0, num_digits);
    return Value(std::trunc(value * factor) / factor);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula