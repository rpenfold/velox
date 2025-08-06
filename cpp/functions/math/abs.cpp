#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value abs_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "ABS",
                                            [](double x) { return std::abs(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula