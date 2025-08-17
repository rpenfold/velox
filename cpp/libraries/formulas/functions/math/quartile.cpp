#include "velox/formulas/functions.h"
#include "velox/formulas/statistical_utils.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the quartile of a data set
 * @ingroup math
 * @param array Array of numeric values
 * @param quart Quartile to return (0-4)
 * @code
 * QUARTILE({1, 2, 3, 4, 5}, 0) -> 1 (minimum)
 * QUARTILE({1, 2, 3, 4, 5}, 1) -> 2 (first quartile, 25th percentile)
 * QUARTILE({1, 2, 3, 4, 5}, 2) -> 3 (median, 50th percentile)
 * QUARTILE({1, 2, 3, 4, 5}, 3) -> 4 (third quartile, 75th percentile)
 * QUARTILE({1, 2, 3, 4, 5}, 4) -> 5 (maximum)
 * @endcode
 * 
 * The QUARTILE function returns the specified quartile of a data set.
 * - quart=0: minimum value (0th percentile)
 * - quart=1: first quartile (25th percentile)
 * - quart=2: median (50th percentile)
 * - quart=3: third quartile (75th percentile)
 * - quart=4: maximum value (100th percentile)
 * - Non-numeric values are ignored
 * - quart must be an integer between 0 and 4 inclusive
 */
Value quartile(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
    if (args.size() != 2) {
        throw std::invalid_argument("QUARTILE function requires exactly 2 arguments");
    }
    
    // Extract array argument
    std::vector<Value> array;
    if (args[0].isArray()) {
        array = args[0].asArray();
    } else {
        array.push_back(args[0]);
    }
    
    // Extract quart argument
    if (!args[1].isNumber()) {
        throw std::invalid_argument("quart must be a number");
    }
    
    double quartDouble = args[1].toNumber();
    if (quartDouble != std::floor(quartDouble) || quartDouble < 0 || quartDouble > 4) {
        throw std::invalid_argument("quart must be an integer between 0 and 4");
    }
    
    int quart = static_cast<int>(quartDouble);
    
    try {
        // Map quartile values to percentiles
        double percentile;
        switch (quart) {
            case 0: // Minimum
                percentile = 0.0;
                break;
            case 1: // First quartile (25th percentile)
                percentile = 0.25;
                break;
            case 2: // Median (50th percentile)
                percentile = 0.5;
                break;
            case 3: // Third quartile (75th percentile)
                percentile = 0.75;
                break;
            case 4: // Maximum
                percentile = 1.0;
                break;
            default:
                throw std::invalid_argument("Invalid quartile value");
        }
        
        return utils::calculatePercentile(array, percentile);
    } catch (const std::exception& e) {
        throw std::invalid_argument(std::string("QUARTILE: ") + e.what());
    }
}

} // namespace builtin
} // namespace functions
} // namespace xl_formula