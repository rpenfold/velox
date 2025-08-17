#include <algorithm>
#include <cctype>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the leftmost characters from a text string
 * @ingroup text
 * @param text Input text
 * @param num_chars Number of characters (optional)
 * @code
 * LEFT("Hello",2) -> "He"
 * @endcode
 */
Value left(const std::vector<Value>& args, const Context& context) {
    return templates::oneOrTwoArgTextFunction(
            args, context, "LEFT", [](const std::string& text, int num_chars) {
                // If num_chars is greater than text length, return entire text
                if (num_chars >= static_cast<int>(text.length())) {
                    return text;
                }

                // Return the leftmost num_chars characters
                return text.substr(0, num_chars);
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula