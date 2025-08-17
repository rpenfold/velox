#include "velox/formulas/functions.h"
#include "velox/formulas/statistical_utils.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value percentile(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
    // PERCENTILE(array, k) - Returns the k-th percentile of values in a range
    // k should be between 0 and 1
    
    if (args.size() != 2) {
        throw std::invalid_argument("PERCENTILE function requires exactly 2 arguments");
    }
    
    // Extract array argument
    std::vector<Value> array;
    if (args[0].isArray()) {
        array = args[0].asArray();
    } else {
        array.push_back(args[0]);
    }
    
    // Extract k argument (percentile value)
    if (!args[1].isNumber()) {
        throw std::invalid_argument("k must be a number");
    }
    
    double k = args[1].toNumber();
    if (k < 0.0 || k > 1.0) {
        throw std::invalid_argument("k must be between 0 and 1");
    }
    
    try {
        return utils::calculatePercentile(array, k);
    } catch (const std::exception& e) {
        throw std::invalid_argument(std::string("PERCENTILE: ") + e.what());
    }
}

} // namespace builtin
} // namespace functions
} // namespace xl_formula