#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value right(const std::vector<Value>& args, const Context& context) {
    return templates::oneOrTwoArgTextFunction(
            args, context, "RIGHT", [](const std::string& text, int num_chars) {
                // If num_chars is greater than text length, return entire text
                if (num_chars >= static_cast<int>(text.length())) {
                    return text;
                }

                // Return the rightmost num_chars characters
                size_t start_pos = text.length() - num_chars;
                return text.substr(start_pos);
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula