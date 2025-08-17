#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Rounds a number to a specified number of digits
 * @ingroup math
 * @param number Number to round
 * @param num_digits Number of digits (optional)
 * @code
 * ROUND(3.14159,2) -> 3.14
 * @endcode
 */
Value round_function(const std::vector<Value>& args, const Context& context) {
    return templates::oneOrTwoArgFunction(args, context, "ROUND",
                                          [](double value, double decimal_places) {
                                              double multiplier = std::pow(10.0, decimal_places);
                                              return std::round(value * multiplier) / multiplier;
                                          });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula