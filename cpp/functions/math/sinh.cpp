#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value sinh_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "SINH",
                                            [](double x) { return std::sinh(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula