#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value cos_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "COS", 
        [](double x) { return std::cos(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula