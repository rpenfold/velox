#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value or_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // OR requires at least one argument
    if (args.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for any errors in arguments
    auto error_check = utils::checkForErrors(args);
    if (!error_check.isEmpty()) {
        return error_check;
    }

    // Convert all arguments to boolean and check if any are TRUE
    for (const auto& arg : args) {
        bool is_true = false;
        
        if (arg.isBoolean()) {
            is_true = arg.asBoolean();
        } else if (arg.canConvertToNumber()) {
            is_true = arg.toNumber() != 0.0;
        } else if (arg.isText()) {
            is_true = !arg.asText().empty();
        } else {
            // Empty values are considered FALSE
            is_true = false;
        }
        
        if (is_true) {
            return Value(true);
        }
    }
    
    return Value(false);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula