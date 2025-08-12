#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns value_if_error if value is an error; otherwise returns value
 * @ingroup logical
 * @param value Value to check for error
 * @param value_if_error Value to return if value is an error
 * @code
 * IFERROR(1/0, 42) -> 42
 * @endcode
 */
Value iferror_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 2, "IFERROR");
    if (!validation.isEmpty()) {
        return validation;
    }

    // Use shared error conditional utility
    return ::xl_formula::conditional::conditionalOnError(args[0], args[1], [](ErrorType) { return true; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula