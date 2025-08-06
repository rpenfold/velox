#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value isblank_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "ISBLANK");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& value = args[0];

    // Return TRUE if the value is empty
    return Value(value.isEmpty());
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula