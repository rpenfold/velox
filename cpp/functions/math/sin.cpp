#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value sin_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "SIN",
                                            [](double x) { return std::sin(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula