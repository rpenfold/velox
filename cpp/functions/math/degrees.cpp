#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value degrees_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "DEGREES",
                                            [](double x) { return x * 180.0 / M_PI; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula