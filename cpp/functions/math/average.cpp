#include <xl-formula/functions.h>
#include <numeric>

namespace xl_formula {
namespace functions {
namespace builtin {

Value average(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // AVERAGE requires at least one argument
    auto validation = utils::validateMinArgs(args, 1, "AVERAGE");
    if (!validation.isEmpty()) {
        return validation;
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::vector<double> numbers;
    numbers.reserve(args.size());

    for (const auto& arg : args) {
        if (!arg.isEmpty() && arg.canConvertToNumber()) {
            numbers.push_back(arg.toNumber());
        }
    }

    // If no numeric values found, return error
    if (numbers.empty()) {
        return Value::error(ErrorType::DIV_ZERO);
    }

    double sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
    return Value(sum / numbers.size());
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula