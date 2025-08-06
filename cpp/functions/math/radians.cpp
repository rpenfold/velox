#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value radians_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "RADIANS",
                                            [](double x) { return x * M_PI / 180.0; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula