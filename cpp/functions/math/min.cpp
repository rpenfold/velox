#include <xl-formula/functions.h>
#include <algorithm>

namespace xl_formula {
namespace functions {
namespace builtin {

Value min(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // MIN requires at least one argument
    auto error = utils::validateMinArgs(args, 1, "MIN");
    if (!error.isEmpty()) {
        return error;
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    Value minValue;
    bool hasValue = false;

    for (const auto& arg : args) {
        if (arg.isEmpty()) {
            continue;  // Skip empty values
        }

        if (!hasValue) {
            minValue = arg;
            hasValue = true;
        } else {
            // Use existing comparison logic from Value class
            if (arg < minValue) {
                minValue = arg;
            }
        }
    }

    // If no valid values found, return 0 (Excel behavior)
    if (!hasValue) {
        return Value(0.0);
    }

    return minValue;
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula