#include <chrono>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief NOW function - returns the current date and time
 * @param args Function arguments (expects 0 arguments)
 * @param context Evaluation context (unused for NOW)
 * @return Current date and time as Value
 */
Value now(const std::vector<Value>& args, const Context& context) {
    return templates::noArgFunction(args, context, "NOW",
                                    []() { return Value(std::chrono::system_clock::now()); });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula