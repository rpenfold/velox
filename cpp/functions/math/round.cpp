#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value round_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    if (args.size() < 1 || args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    const auto& number_arg = args[0];
    if (number_arg.isError()) {
        return number_arg;
    }

    auto number = utils::toNumberSafe(number_arg, "ROUND");
    if (number.isError()) {
        return number;
    }

    int decimal_places = 0;
    if (args.size() == 2) {
        const auto& places_arg = args[1];
        if (places_arg.isError()) {
            return places_arg;
        }

        auto places = utils::toNumberSafe(places_arg, "ROUND");
        if (places.isError()) {
            return places;
        }

        decimal_places = static_cast<int>(places.asNumber());
    }

    double value = number.asNumber();
    double multiplier = std::pow(10.0, decimal_places);
    double rounded = std::round(value * multiplier) / multiplier;

    return Value(rounded);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula