#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value tan_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "TAN",
                                            [](double x) { return std::tan(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula