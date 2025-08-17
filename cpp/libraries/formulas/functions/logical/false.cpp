#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the logical value FALSE
 * @ingroup logical
 * @code
 * FALSE() -> FALSE
 * @endcode
 */
Value false_function(const std::vector<Value>& args, const Context& context) {
    (void)args;     // Unused parameter
    (void)context;  // Unused parameter

    return Value(false);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula