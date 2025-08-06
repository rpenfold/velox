#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value len(const std::vector<Value>& args, const Context& context) {
    return templates::singleTextFunction(args, context, "LEN", [](const std::string& text) {
        return static_cast<double>(text.length());
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula