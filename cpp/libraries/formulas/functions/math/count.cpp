#include <velox/formulas/functions.h>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Counts the number of numeric values among the arguments
 * @ingroup math
 * @param value1 First value or range
 * @param value2 Additional values (optional, variadic)
 * @code
 * COUNT(1, 2, "x") -> 2
 * @endcode
 */
Value count(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // COUNT accepts zero or more arguments
    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    int count = 0;

    for (const auto& arg : args) {
        if (arg.isNumber()) {
            count++;
        }
        // COUNT only counts numeric values (Excel behavior)
        // Text, boolean, empty, and error values are ignored
    }

    return Value(static_cast<double>(count));
}

/**
 * @brief Counts the number of non-empty values among the arguments
 * @ingroup math
 * @param value1 First value or range
 * @param value2 Additional values (optional, variadic)
 * @code
 * COUNTA(1, "x", "") -> 2
 * @endcode
 */
Value counta(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // COUNTA accepts zero or more arguments
    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    int count = 0;

    for (const auto& arg : args) {
        if (!arg.isEmpty()) {
            count++;
        }
        // COUNTA counts all non-empty values (Excel behavior)
        // Numbers, text, booleans, and errors are all counted
    }

    return Value(static_cast<double>(count));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula