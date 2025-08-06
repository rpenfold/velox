#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value sum(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    if (args.empty()) {
        return Value(0.0);
    }

    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }

    double total = 0.0;

    for (const auto& arg : args) {
        if (arg.canConvertToNumber()) {
            total += arg.toNumber();
        }
        // Ignore non-numeric values (Excel behavior)
    }

    return Value(total);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula