#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

// CHOOSE(index, value1, [value2], ...)
Value choose(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.size() < 2) return Value::error(ErrorType::VALUE_ERROR);
    auto idxV = utils::toNumberSafe(args[0], "CHOOSE");
    if (idxV.isError()) return idxV;
    int index = static_cast<int>(idxV.asNumber());
    if (index < 1 || static_cast<size_t>(index) >= args.size()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    return args[index];
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


