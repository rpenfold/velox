#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value concatenate(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }

    std::string result;
    for (const auto& arg : args) {
        result += arg.toString();
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula