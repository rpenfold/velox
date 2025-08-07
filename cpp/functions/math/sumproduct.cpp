#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value sumproduct(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Suppress unused parameter warning

    if (args.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    try {
        // Convert all arguments to numeric values, treating arrays as single values for now
        // In a full implementation, this would handle ranges/arrays properly
        std::vector<double> numbers;
        numbers.reserve(args.size());

        for (const auto& arg : args) {
            if (arg.isError()) {
                return arg;
            }

            auto numValue = utils::toNumberSafe(arg, "SUMPRODUCT");
            if (numValue.isError()) {
                return numValue;
            }

            numbers.push_back(numValue.asNumber());
        }

        // Calculate product of all arguments
        double result = 1.0;
        for (double num : numbers) {
            result *= num;
        }

        return Value(result);

    } catch (const std::exception& e) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula