#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief SIGN function - returns the sign of a number
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for SIGN)
 * @return 1 if positive, -1 if negative, 0 if zero
 */
Value sign(const std::vector<Value>& args, const Context& context) {
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

    if (value > 0.0) {
        return Value(1.0);
    } else if (value < 0.0) {
        return Value(-1.0);
    } else {
        return Value(0.0);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula