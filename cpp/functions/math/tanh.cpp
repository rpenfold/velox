#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value tanh_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "TANH",
                                            [](double x) { return std::tanh(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula