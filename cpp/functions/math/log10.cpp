#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value log10_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "LOG10", 
        [](double x) { 
            if (x <= 0.0) {
                throw std::runtime_error("Domain error");
            }
            return std::log10(x); 
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula