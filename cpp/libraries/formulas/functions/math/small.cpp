#include "velox/formulas/functions.h"
#include "velox/formulas/statistical_utils.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the k-th smallest value in a dataset
 * @ingroup math
 * @param array Array of numeric values to search
 * @param k Position of the smallest value to return (1-based)
 * @code
 * SMALL({1, 2, 3, 4, 5}, 1) -> 1
 * SMALL({1, 2, 3, 4, 5}, 2) -> 2
 * @endcode
 * 
 * The SMALL function returns the k-th smallest value from an array of numbers.
 * - k=1 returns the smallest value (minimum)
 * - k=2 returns the second smallest value, and so on
 * - k must be a positive integer between 1 and the count of numeric values
 * - Non-numeric values in the array are ignored
 */
Value small(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
    if (args.size() != 2) {
        throw std::invalid_argument("SMALL function requires exactly 2 arguments");
    }
    
    // Extract array argument
    std::vector<Value> array;
    if (args[0].isArray()) {
        array = args[0].asArray();
    } else {
        array.push_back(args[0]);
    }
    
    // Extract k argument
    if (!args[1].isNumber()) {
        throw std::invalid_argument("k must be a number");
    }
    
    double kDouble = args[1].toNumber();
    if (kDouble != std::floor(kDouble) || kDouble < 1) {
        throw std::invalid_argument("k must be a positive integer");
    }
    
    size_t k = static_cast<size_t>(kDouble);
    
    try {
        // Use quickSelect with ascending order (true) to get k-th smallest
        // Convert to 0-based index
        return utils::quickSelect(array, k - 1, true);
    } catch (const std::exception& e) {
        throw std::invalid_argument(std::string("SMALL: ") + e.what());
    }
}

} // namespace builtin
} // namespace functions
} // namespace xl_formula