#include <string>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Counts the number of values that meet a condition
 * @ingroup math
 * @param range Values to test
 * @param criteria Condition to evaluate
 * @code
 * COUNTIF({1,2,3}, ">=2") -> 2
 * @endcode
 */
Value countif(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }

    // Validate argument count (at least 2 arguments)
    if (args.size() < 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Last argument is criteria, all others are values to test
    Value criteria = args.back();

    int count = 0;

    // Test each value against criteria
    for (size_t i = 0; i < args.size() - 1; ++i) {
        if (::xl_formula::conditional::evaluateCriteria(args[i], criteria)) {
            count++;
        }
    }

    return Value(static_cast<double>(count));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula