#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the logical value TRUE
 * @ingroup logical
 * @code
 * TRUE() -> TRUE
 * @endcode
 */
Value true_function(const std::vector<Value>& args, const Context& context) {
    (void)args;     // Unused parameter
    (void)context;  // Unused parameter

    return Value(true);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula