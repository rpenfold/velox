#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief LOWER function - converts text to lowercase
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for LOWER)
 * @return Text converted to lowercase
 */
Value lower(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "LOWER");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& arg = args[0];
    if (arg.isError()) {
        return arg;
    }

    std::string text = arg.toString();
    
    // Convert to lowercase
    std::transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    return Value(text);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula