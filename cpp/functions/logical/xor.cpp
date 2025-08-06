#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value xor_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // XOR requires at least one argument
    if (args.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for any errors in arguments
    auto error_check = utils::checkForErrors(args);
    if (!error_check.isEmpty()) {
        return error_check;
    }

    // Count how many arguments are TRUE
    int true_count = 0;

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
            true_count++;
        }
    }

    // XOR returns TRUE if an odd number of arguments are TRUE
    return Value((true_count % 2) == 1);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula