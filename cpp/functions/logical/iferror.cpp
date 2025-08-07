#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value iferror_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 2, "IFERROR");
    if (!validation.isEmpty()) {
        return validation;
    }

    // Use shared error conditional utility
    return ::xl_formula::conditional::conditionalOnError(args[0], args[1], [](ErrorType) { return true; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula