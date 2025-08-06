#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value bitor_function(const std::vector<Value>& args, const Context& context) {
    return templates::bitwiseFunction(
            args, context, "BITOR",
            [](long long value1, long long value2) -> long long { return value1 | value2; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula