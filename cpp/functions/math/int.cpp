#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value int_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "INT",
                                            [](double x) { return std::floor(x); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula