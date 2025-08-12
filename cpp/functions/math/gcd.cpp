#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the greatest common divisor
 * @ingroup math
 * @param number1 First integer
 * @param number2 Additional integers (optional, variadic)
 * @code
 * GCD(12, 18) -> 6
 * @endcode
 */
Value gcd(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // GCD requires at least 1 argument
    if (args.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert all arguments to numbers
    std::vector<double> numbers;
    for (const auto& arg : args) {
        auto num = utils::toNumberSafe(arg, "GCD");
        if (num.isError()) {
            return num;
        }
        numbers.push_back(num.asNumber());
    }

    // Helper function to calculate GCD of two numbers
    auto gcd_two = [](long long a, long long b) -> long long {
        a = std::abs(a);
        b = std::abs(b);
        while (b != 0) {
            long long temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    };

    long long result = static_cast<long long>(std::abs(numbers[0]));
    for (size_t i = 1; i < numbers.size(); ++i) {
        long long val = static_cast<long long>(std::abs(numbers[i]));
        result = gcd_two(result, val);
        if (result == 1) {
            break;  // GCD cannot be smaller than 1
        }
    }

    return Value(static_cast<double>(result));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula