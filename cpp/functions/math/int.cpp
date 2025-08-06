#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief INT function - rounds a number down to the nearest integer
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for INT)
 * @return Integer part of the number (rounded down)
 */
Value int_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
        // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }
    
    // Validate argument count
    if (args.size() != 1) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    // Convert argument to number
    if (!args[0].canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    double value = args[0].toNumber();

    // Use floor to round down to nearest integer
    return Value(std::floor(value));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula