#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the largest value among the arguments
 * @ingroup math
 * @param number1 First number (optional)
 * @param number2 Additional numbers (variadic)
 * @code
 * MAX(10,20,5) -> 20
 * @endcode
 */
Value max(const std::vector<Value>& args, const Context& context) {
    return templates::minMaxFunction(args, context, "MAX", 
        [](const Value& a, const Value& b) { return a > b; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula