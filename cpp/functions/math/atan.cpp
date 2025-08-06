#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value atan_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "ATAN", 
        [](double x) { return std::atan(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula