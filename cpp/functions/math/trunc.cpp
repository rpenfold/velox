#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

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