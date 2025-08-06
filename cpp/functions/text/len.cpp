#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value len(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "LEN");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& arg = args[0];
    if (arg.isError()) {
        return arg;
    }

    std::string text = arg.toString();
    return Value(static_cast<double>(text.length()));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula