#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns TRUE if the value is a number or convertible to a number
 * @ingroup logical
 * @param value Value to test
 * @code
 * ISNUMBER("123") -> TRUE
 * @endcode
 */
Value isnumber_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "ISNUMBER");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& value = args[0];

    // Return TRUE if the value is a number or can be converted to a number
    return Value(value.isNumber() || value.canConvertToNumber());
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula