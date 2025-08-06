#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value false_function(const std::vector<Value>& args, const Context& context) {
    (void)args;     // Unused parameter
    (void)context;  // Unused parameter

    return Value(false);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula