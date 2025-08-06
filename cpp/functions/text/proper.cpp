#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value proper(const std::vector<Value>& args, const Context& context) {
    return templates::singleTextFunction(args, context, "PROPER", [](const std::string& text) {
        if (text.empty()) {
            return text;
        }

        std::string result = text;

        // Convert to lowercase first
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        // Capitalize first character
        if (!result.empty()) {
            result[0] = std::toupper(result[0]);
        }

        // Capitalize first letter after each space
        for (size_t i = 1; i < result.length(); ++i) {
            if (std::isspace(result[i - 1]) && !std::isspace(result[i])) {
                result[i] = std::toupper(result[i]);
            }
        }

        return result;
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula