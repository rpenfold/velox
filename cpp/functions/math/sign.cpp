#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value sign(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "SIGN", [](double x) {
        if (x > 0.0)
            return 1.0;
        if (x < 0.0)
            return -1.0;
        return 0.0;
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula