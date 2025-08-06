#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value not_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "NOT");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& arg = args[0];

    if (arg.isError()) {
        return arg;
    }

    // Convert argument to boolean and return the opposite
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
    
    return Value(!is_true);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula