#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value exp_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "EXP", 
        [](double x) { return std::exp(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula