#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief UPPER function - converts text to uppercase
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for UPPER)
 * @return Text converted to uppercase
 */
Value upper(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "UPPER");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& arg = args[0];
    if (arg.isError()) {
        return arg;
    }

    std::string text = arg.toString();
    
    // Convert to uppercase
    std::transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    
    return Value(text);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula