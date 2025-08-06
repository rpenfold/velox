#include <xl-formula/functions.h>
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

Value sqrt_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // SQRT requires exactly one argument
    auto error = utils::validateArgCount(args, 1, "SQRT");
    if (!error.isEmpty()) {
        return error;
    }

    // Convert to number safely
    auto numberValue = utils::toNumberSafe(args[0], "SQRT");
    if (numberValue.isError()) {
        return numberValue;
    }

    double value = numberValue.asNumber();

    // SQRT of negative number returns #NUM! error
    if (value < 0.0) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    return Value(std::sqrt(value));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula