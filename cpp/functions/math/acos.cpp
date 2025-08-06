#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value acos_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "ACOS", 
        [](double x) { 
            if (x < -1.0 || x > 1.0) {
                throw std::runtime_error("Domain error");
            }
            return std::acos(x); 
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula