#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value abs_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "ABS");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& arg = args[0];
    if (arg.isError()) {
        return arg;
    }

    auto number = utils::toNumberSafe(arg, "ABS");
    if (number.isError()) {
        return number;
    }

    return Value(std::abs(number.asNumber()));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula