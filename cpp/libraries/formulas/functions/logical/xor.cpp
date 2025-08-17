#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns TRUE if an odd number of arguments are TRUE
 * @ingroup logical
 * @param logical1 First logical value
 * @param logical2 Additional logical values (optional, variadic)
 * @code
 * XOR(TRUE, TRUE, TRUE) -> TRUE
 * @endcode
 */
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