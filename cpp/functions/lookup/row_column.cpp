#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

// ROW([reference]) -> since we don't have sheet references, return 1 for no-arg, else 1
Value row_function(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.size() > 1) return Value::error(ErrorType::VALUE_ERROR);
    return Value(1.0);
}

// COLUMN([reference]) -> return 1 similarly
Value column_function(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.size() > 1) return Value::error(ErrorType::VALUE_ERROR);
    return Value(1.0);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


