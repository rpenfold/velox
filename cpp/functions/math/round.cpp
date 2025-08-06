#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

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