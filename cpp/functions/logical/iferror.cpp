#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value iferror_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 2, "IFERROR");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& value = args[0];
    const auto& value_if_error = args[1];

    // If the first argument is an error, return the second argument
    if (value.isError()) {
        return value_if_error;
    }
    
    // Otherwise, return the first argument
    return value;
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula