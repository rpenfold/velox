#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the value of pi (π)
 * @ingroup math
 * @code
 * PI() -> 3.14159
 * @endcode
 */
Value pi(const std::vector<Value>& args, const Context& context) {
    return templates::noArgFunction(args, context, "PI", []() { return Value(M_PI); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula