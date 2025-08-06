#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value trim(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "TRIM");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& arg = args[0];
    if (arg.isError()) {
        return arg;
    }

    std::string text = arg.toString();

    // Trim leading whitespace
    text.erase(text.begin(), std::find_if(text.begin(), text.end(),
                                          [](unsigned char ch) { return !std::isspace(ch); }));

    // Trim trailing whitespace
    text.erase(std::find_if(text.rbegin(), text.rend(),
                            [](unsigned char ch) { return !std::isspace(ch); })
                       .base(),
               text.end());

    return Value(text);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula