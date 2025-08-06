#include <xl-formula/functions.h>

namespace xl_formula {
namespace functions {
namespace builtin {

Value average(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // AVERAGE requires at least one argument
    auto error = utils::validateMinArgs(args, 1, "AVERAGE");
    if (!error.isEmpty()) {
        return error;
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    double sum = 0.0;
    int count = 0;

    for (const auto& arg : args) {
        if (arg.isEmpty()) {
            continue;  // Skip empty values
        }

        if (arg.canConvertToNumber()) {
            sum += arg.toNumber();
            count++;
        }
        // Non-numeric values are ignored in AVERAGE (Excel behavior)
    }

    // If no numeric values found, return error
    if (count == 0) {
        return Value::error(ErrorType::DIV_ZERO);
    }

    return Value(sum / count);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula