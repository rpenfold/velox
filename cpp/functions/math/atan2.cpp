#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value atan2_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // ATAN2 requires exactly two arguments
    auto error = utils::validateArgCount(args, 2, "ATAN2");
    if (!error.isEmpty()) {
        return error;
    }

    // Convert both arguments to numbers safely
    auto yValue = utils::toNumberSafe(args[0], "ATAN2");
    if (yValue.isError()) {
        return yValue;
    }

    auto xValue = utils::toNumberSafe(args[1], "ATAN2");
    if (xValue.isError()) {
        return xValue;
    }

    double y = yValue.asNumber();
    double x = xValue.asNumber();

    // ATAN2 returns the angle whose tangent is y/x
    double result = std::atan2(y, x);

    // Check for overflow/underflow
    if (!std::isfinite(result)) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula