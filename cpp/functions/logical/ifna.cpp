#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value ifna_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 2, "IFNA");
    if (!validation.isEmpty()) {
        return validation;
    }

    // Use shared error conditional utility with NA-specific predicate
    return ::xl_formula::conditional::conditionalOnError(args[0], args[1], 
                                   [](ErrorType error) { return error == ErrorType::NA_ERROR; });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula