#include <chrono>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the current date and time
 * @ingroup datetime
 * @code
 * NOW() -> 2025-08-11 10:00:00
 * @endcode
 */
Value now(const std::vector<Value>& args, const Context& context) {
    return templates::noArgFunction(args, context, "NOW",
                                    []() { return Value(std::chrono::system_clock::now()); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula