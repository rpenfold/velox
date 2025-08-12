#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns TRUE if the value is an error
 * @ingroup logical
 * @param value Value to test
 * @code
 * ISERROR(#DIV/0!) -> TRUE
 * @endcode
 */
Value iserror_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "ISERROR");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& value = args[0];

    // Return TRUE if the value is an error
    return Value(value.isError());
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula