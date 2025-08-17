#include "velox/formulas/functions.h"
#include "velox/formulas/statistical_utils.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value rank(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
    // RANK(number, ref, [order]) - Returns the rank of a number in a list of numbers
    // order: 0 = descending (default), 1 = ascending
    
    if (args.size() < 2 || args.size() > 3) {
        throw std::invalid_argument("RANK function requires 2 or 3 arguments");
    }
    
    // Extract number argument
    if (!args[0].isNumber()) {
        throw std::invalid_argument("Number must be numeric");
    }
    Value number = args[0];
    
    // Extract reference array argument
    std::vector<Value> array;
    if (args[1].isArray()) {
        array = args[1].asArray();
    } else {
        array.push_back(args[1]);
    }
    
    // Extract order argument (optional)
    bool ascending = false; // Default is descending (0)
    if (args.size() == 3) {
        if (!args[2].isNumber()) {
            throw std::invalid_argument("Order must be numeric");
        }
        double orderValue = args[2].toNumber();
        if (orderValue != 0.0 && orderValue != 1.0) {
            throw std::invalid_argument("Order must be 0 (descending) or 1 (ascending)");
        }
        ascending = (orderValue == 1.0);
    }
    
    try {
        return utils::calculateRank(number, array, ascending);
    } catch (const std::exception& e) {
        throw std::invalid_argument(std::string("RANK: ") + e.what());
    }
}

} // namespace builtin
} // namespace functions
} // namespace xl_formula