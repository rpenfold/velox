#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief PROPER function - capitalizes the first letter of each word
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for PROPER)
 * @return Text with first letter of each word capitalized
 */
Value proper(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "PROPER");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& arg = args[0];
    if (arg.isError()) {
        return arg;
    }

    std::string text = arg.toString();
    
    if (text.empty()) {
        return Value(text);
    }
    
    // Convert to lowercase first
    std::transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    // Capitalize first character
    if (!text.empty()) {
        text[0] = std::toupper(text[0]);
    }
    
    // Capitalize first letter after each space
    for (size_t i = 1; i < text.length(); ++i) {
        if (std::isspace(text[i - 1]) && !std::isspace(text[i])) {
            text[i] = std::toupper(text[i]);
        }
    }
    
    return Value(text);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula