#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns a bitwise XOR of two numbers
 * @ingroup engineering
 * @param number1 First number
 * @param number2 Second number
 * @code
 * BITXOR(5, 3) -> 6
 * @endcode
 */
Value bitxor_function(const std::vector<Value>& args, const Context& context) {
    return templates::bitwiseFunction(
            args, context, "BITXOR",
            [](long long value1, long long value2) -> long long { return value1 ^ value2; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula