#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns a bitwise AND of two numbers
 * @ingroup engineering
 * @param number1 First number
 * @param number2 Second number
 * @code
 * BITAND(5, 3) -> 1
 * @endcode
 */
Value bitand_function(const std::vector<Value>& args, const Context& context) {
    return templates::bitwiseFunction(
            args, context, "BITAND",
            [](long long value1, long long value2) -> long long { return value1 & value2; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula