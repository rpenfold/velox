#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief IFS function - checks multiple conditions and returns corresponding result for first TRUE condition
 * @param args Function arguments (condition1, result1, [condition2, result2, ...])
 * @param context Evaluation context (unused for IFS)
 * @return The result corresponding to the first TRUE condition, or #N/A if none are TRUE
 * 
 * Excel syntax: IFS(condition1, result1, [condition2, result2, ...])
 * - condition1, condition2, ...: Logical conditions to test
 * - result1, result2, ...: Results to return if corresponding condition is TRUE
 * 
 * The function requires at least 2 arguments (condition1, result1)
 * Arguments come in pairs (condition, result)
 * Returns #N/A error if no conditions are TRUE
 */
Value ifs_function(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // Validate minimum argument count (condition1, result1)
    auto minValidation = utils::validateMinArgs(args, 2, "IFS");
    if (!minValidation.isEmpty()) {
        return minValidation;
    }
    
    // Validate that we have an even number of arguments (pairs)
    if (args.size() % 2 != 0) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }
    
    // Number of condition/result pairs
    size_t numPairs = args.size() / 2;
    
    // Check each condition/result pair
    for (size_t i = 0; i < numPairs; ++i) {
        const Value& condition = args[i * 2];         // condition to test
        const Value& resultValue = args[i * 2 + 1];   // result if condition is TRUE
        
        // Convert condition to boolean
        bool conditionResult = false;
        
        switch (condition.getType()) {
            case ValueType::BOOLEAN:
                conditionResult = condition.asBoolean();
                break;
            case ValueType::NUMBER:
                // In Excel, 0 is FALSE, any non-zero number is TRUE
                conditionResult = (condition.asNumber() != 0.0);
                break;
            case ValueType::TEXT:
                // Text values cannot be evaluated as logical in IFS
                return Value::error(ErrorType::VALUE_ERROR);
            case ValueType::ERROR:
                // Propagate error from condition
                return condition;
            case ValueType::DATE:
                // Dates are treated as numbers (non-zero = TRUE)
                conditionResult = true; // Dates are always non-zero
                break;
            case ValueType::ARRAY:
                // Arrays not supported in IFS conditions
                return Value::error(ErrorType::VALUE_ERROR);
            case ValueType::EMPTY:
                // Empty values are treated as FALSE
                conditionResult = false;
                break;
        }
        
        if (conditionResult) {
            return resultValue;
        }
    }
    
    // No condition was TRUE, return #N/A error
    return Value::error(ErrorType::NA_ERROR);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
