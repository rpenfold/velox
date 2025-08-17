#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the sign of a number (1, 0, or -1)
 * @ingroup math
 * @param number The number to test
 * @code
 * SIGN(-3) -> -1
 * @endcode
 */
Value sign(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "SIGN", [](double x) {
        if (x > 0.0)
            return 1.0;
        if (x < 0.0)
            return -1.0;
        return 0.0;
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula