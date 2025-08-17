#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief T function - converts value to text, returns empty string for non-text values
 * @param args Function arguments (value)
 * @param context Evaluation context (unused for T)
 * @return Text representation if input is text, empty string otherwise
 *
 * Excel behavior:
 * - If value is text, returns the text unchanged
 * - If value is not text (number, boolean, error), returns empty string ""
 * - This differs from toString() which converts all types to text
 */
Value t_function(const std::vector<Value>& args, const Context& context) {
    (void)context;

    auto countValidation = utils::validateArgCount(args, 1, "T");
    if (!countValidation.isEmpty()) {
        return countValidation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    const Value& value = args[0];

    // T function returns text only if the input is already text
    // All other types (numbers, booleans, errors) return empty string
    if (value.isText()) {
        return value;
    } else {
        return Value("");  // Empty string for non-text values
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
