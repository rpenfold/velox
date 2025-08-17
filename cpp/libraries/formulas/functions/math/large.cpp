#include "velox/formulas/functions.h"
#include "velox/formulas/statistical_utils.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the k-th largest value in a dataset
 * @ingroup math
 * @param array Array of numeric values to search
 * @param k Position of the largest value to return (1-based)
 * @code
 * LARGE({1, 2, 3, 4, 5}, 1) -> 5
 * LARGE({1, 2, 3, 4, 5}, 2) -> 4
 * @endcode
 * 
 * The LARGE function returns the k-th largest value from an array of numbers.
 * - k=1 returns the largest value (maximum)
 * - k=2 returns the second largest value, and so on
 * - k must be a positive integer between 1 and the count of numeric values
 * - Non-numeric values in the array are ignored
 */
Value large(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
    if (args.size() != 2) {
        throw std::invalid_argument("LARGE function requires exactly 2 arguments");
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
        // Use quickSelect with descending order (false) to get k-th largest
        // Convert to 0-based index
        return utils::quickSelect(array, k - 1, false);
    } catch (const std::exception& e) {
        throw std::invalid_argument(std::string("LARGE: ") + e.what());
    }
}

} // namespace builtin
} // namespace functions
} // namespace xl_formula