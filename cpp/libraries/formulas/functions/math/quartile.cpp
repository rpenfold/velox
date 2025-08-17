#include "velox/formulas/functions.h"
#include "velox/formulas/statistical_utils.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value quartile(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
    // QUARTILE(array, quart) - Returns the quartile of a data set
    // quart: 0 = minimum, 1 = first quartile, 2 = median, 3 = third quartile, 4 = maximum
    
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