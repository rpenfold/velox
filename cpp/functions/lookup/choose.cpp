#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Chooses a value from a list of values
 * @name CHOOSE
 * @category lookup
 * @param index 1-based index of the value to return
 * @param value1 First value
 * @param value2 Additional values (variadic)
 * @code
 * CHOOSE(2,"A","B","C") -> "B"
 * @endcode
 */
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


