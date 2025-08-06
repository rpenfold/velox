#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value upper(const std::vector<Value>& args, const Context& context) {
    return templates::singleTextFunction(args, context, "UPPER",
        [](const std::string& text) {
            std::string result = text;
            std::transform(result.begin(), result.end(), result.begin(),
                          [](unsigned char c) { return std::toupper(c); });
            return result;
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula