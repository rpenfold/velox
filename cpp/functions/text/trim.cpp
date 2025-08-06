#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value trim(const std::vector<Value>& args, const Context& context) {
    return templates::singleTextFunction(args, context, "TRIM",
        [](const std::string& text) {
            std::string result = text;
            
            // Trim leading whitespace
            result.erase(result.begin(), std::find_if(result.begin(), result.end(),
                                              [](unsigned char ch) { return !std::isspace(ch); }));
            
            // Trim trailing whitespace
            result.erase(std::find_if(result.rbegin(), result.rend(),
                              [](unsigned char ch) { return !std::isspace(ch); })
                         .base(),
                     result.end());
            
            return result;
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula