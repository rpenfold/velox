#include <string>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Removes all nonprintable characters from text (ASCII < 32)
 * @ingroup text
 * @param text The text from which to remove nonprintable characters
 * @code
 * CLEAN("Hello\nWorld") -> "HelloWorld"
 * @endcode
 */
Value clean(const std::vector<Value>& args, const Context& context) {
    (void)context;

    auto countValidation = utils::validateArgCount(args, 1, "CLEAN");
    if (!countValidation.isEmpty()) {
        return countValidation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::string text = args[0].toString();
    std::string result;
    result.reserve(text.size());
    for (unsigned char ch : text) {
        if (ch >= 32) {
            result.push_back(static_cast<char>(ch));
        }
    }
    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
