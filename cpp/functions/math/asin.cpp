#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value asin_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "ASIN", [](double x) {
        if (x < -1.0 || x > 1.0) {
            throw std::runtime_error("Domain error");
        }
        return std::asin(x);
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula