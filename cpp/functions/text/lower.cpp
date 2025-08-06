#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value lower(const std::vector<Value>& args, const Context& context) {
    return templates::singleTextFunction(args, context, "LOWER",
        [](const std::string& text) {
            std::string result = text;
            std::transform(result.begin(), result.end(), result.begin(),
                          [](unsigned char c) { return std::tolower(c); });
            return result;
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula