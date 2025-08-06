#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value cosh_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "COSH", 
        [](double x) { return std::cosh(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula