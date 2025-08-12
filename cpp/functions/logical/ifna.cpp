#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns value_if_na if value is the #N/A error; otherwise returns value
 * @ingroup logical
 * @param value Value to check for #N/A
 * @param value_if_na Value to return if value is #N/A
 * @code
 * IFNA(#N/A, 0) -> 0
 * @endcode
 */
Value ifna_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 2, "IFNA");
    if (!validation.isEmpty()) {
        return validation;
    }

    // Use shared error conditional utility with NA-specific predicate
    return ::xl_formula::conditional::conditionalOnError(
            args[0], args[1], [](ErrorType error) { return error == ErrorType::NA_ERROR; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula