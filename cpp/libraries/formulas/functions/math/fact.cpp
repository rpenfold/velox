#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the factorial of a number
 * @ingroup math
 * @param number Non-negative integer (n <= 170)
 * @code
 * FACT(5) -> 120
 * @endcode
 */
Value fact(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "FACT", [](double x) -> double {
        // Check for non-negative integer
        if (x < 0) {
            throw std::runtime_error("FACT: Argument must be non-negative");
        }

        long long n = static_cast<long long>(x);
        if (std::abs(x - n) > 1e-10) {
            throw std::runtime_error("FACT: Argument must be an integer");
        }

        // Excel limits factorial to 170! (beyond that returns #NUM!)
        if (n > 170) {
            throw std::runtime_error("FACT: Argument too large");
        }

        // Calculate factorial
        double result = 1.0;
        for (long long i = 2; i <= n; ++i) {
            result *= i;
        }

        return result;
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula