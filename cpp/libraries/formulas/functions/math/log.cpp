#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the logarithm of a number to a specified base (default base 10)
 * @ingroup math
 * @param number Positive number
 * @param base Base of the logarithm (optional, default: 10)
 * @code
 * LOG(8, 2) -> 3
 * @endcode
 */
Value log_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // LOG requires 1 or 2 arguments
    if (args.size() < 1 || args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Convert first argument to number
    auto number = utils::toNumberSafe(args[0], "LOG");
    if (number.isError()) {
        return number;
    }

    double x = number.asNumber();
    if (x <= 0.0) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    // If second argument provided, use it as base, otherwise use base 10
    double base = 10.0;
    if (args.size() == 2) {
        auto baseValue = utils::toNumberSafe(args[1], "LOG");
        if (baseValue.isError()) {
            return baseValue;
        }
        base = baseValue.asNumber();
        if (base <= 0.0 || base == 1.0) {
            return Value::error(ErrorType::NUM_ERROR);
        }
    }

    try {
        double result = std::log(x) / std::log(base);
        return Value(result);
    } catch (const std::runtime_error&) {
        return Value::error(ErrorType::NUM_ERROR);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula