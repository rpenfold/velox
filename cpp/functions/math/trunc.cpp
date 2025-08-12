#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Truncates a number to an integer or to a specified number of digits
 * @ingroup math
 * @param number The number to truncate
 * @param num_digits Number of digits (optional; negative truncates to tens, hundreds, ...)
 * @code
 * TRUNC(3.14159, 2) -> 3.14
 * @endcode
 */
Value trunc_function(const std::vector<Value>& args, const Context& context) {
    return templates::oneOrTwoArgFunction(args, context, "TRUNC",
                                          [](double value, double num_digits) {
                                              int digits = static_cast<int>(num_digits);

                                              // Handle negative digits (truncate to tens, hundreds,
                                              // etc.)
                                              if (digits < 0) {
                                                  double factor = std::pow(10.0, -digits);
                                                  return std::trunc(value / factor) * factor;
                                              }

                                              // Handle positive digits (truncate to decimal places)
                                              double factor = std::pow(10.0, digits);
                                              return std::trunc(value * factor) / factor;
                                          });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula