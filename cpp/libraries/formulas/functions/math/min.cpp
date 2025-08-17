#include <velox/formulas/functions.h>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the smallest value among the arguments
 * @ingroup math
 * @param number1 First number (optional)
 * @param number2 Additional numbers (variadic)
 * @code
 * MIN(10,20,5) -> 5
 * @endcode
 */
Value min(const std::vector<Value>& args, const Context& context) {
    return templates::minMaxFunction(args, context, "MIN", 
        [](const Value& a, const Value& b) { return a < b; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula