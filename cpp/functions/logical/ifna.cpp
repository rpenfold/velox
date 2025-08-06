#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value ifna_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 2, "IFNA");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& value = args[0];
    const auto& value_if_na = args[1];

    // If the first argument is #N/A error, return the second argument
    if (value.isError() && value.asError() == ErrorType::NA_ERROR) {
        return value_if_na;
    }

    // Otherwise, return the first argument
    return value;
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula