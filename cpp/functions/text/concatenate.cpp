#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value concatenate(const std::vector<Value>& args, const Context& context) {
    return templates::multiArgFunction(args, context, "CONCATENATE",
        [](const std::vector<Value>& args) {
            std::string result;
            for (const auto& arg : args) {
                result += arg.toString();
            }
            return result;
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula