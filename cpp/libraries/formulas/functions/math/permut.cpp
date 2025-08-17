#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the number of permutations for a given number of items
 * @ingroup math
 * @param n Total number of items
 * @param k Number of items to arrange
 * @code
 * PERMUT(5, 3) -> 60
 * @endcode
 */
Value permut(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // PERMUT requires exactly 2 arguments
    if (args.size() != 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert arguments to numbers
    auto n_val = utils::toNumberSafe(args[0], "PERMUT");
    if (n_val.isError())
        return n_val;
    auto k_val = utils::toNumberSafe(args[1], "PERMUT");
    if (k_val.isError())
        return k_val;

    double n = n_val.asNumber();
    double k = k_val.asNumber();

    try {
        // Convert to integers
        long long n_int = static_cast<long long>(n);
        long long k_int = static_cast<long long>(k);

        // Validate inputs
        if (n < 0 || k < 0) {
            throw std::runtime_error("PERMUT: Arguments must be non-negative");
        }

        if (std::abs(n - n_int) > 1e-10 || std::abs(k - k_int) > 1e-10) {
            throw std::runtime_error("PERMUT: Arguments must be integers");
        }

        if (k > n) {
            throw std::runtime_error("PERMUT: k cannot be greater than n");
        }

        // Calculate permutation: P(n,k) = n! / (n-k)! = n * (n-1) * ... * (n-k+1)
        double result = 1.0;
        for (long long i = 0; i < k_int; ++i) {
            result *= (n_int - i);
        }

        return result;
    } catch (const std::runtime_error&) {
        return Value::error(ErrorType::NUM_ERROR);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula