#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Joins two or more text strings into one string
 * @ingroup text
 * @param text1 The first text string
 * @param text2 Additional text strings (optional, variadic)
 * @see CONCAT
 * @code
 * CONCATENATE("Hello", " ", "World") -> "Hello World"
 * @endcode
 */
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