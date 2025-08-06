#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value pi(const std::vector<Value>& args, const Context& context) {
    return templates::noArgFunction(args, context, "PI",
        []() { return Value(M_PI); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula