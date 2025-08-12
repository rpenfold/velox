#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Compares an expression against multiple values and returns corresponding result
 * @ingroup logical
 * @param expression The value to compare against
 * @param value1 First value to compare, followed by result1
 * @param result1 Result to return if value1 matches
 * @param value2/result2 Additional pairs (optional, variadic)
 * @param default Optional default value if no match
 * @code
 * SWITCH(2, 1, "One", 2, "Two", 3, "Three") -> "Two"
 * @endcode
 */
Value switch_function(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // Validate minimum argument count (expression, value1, result1)
    auto minValidation = utils::validateMinArgs(args, 3, "SWITCH");
    if (!minValidation.isEmpty()) {
        return minValidation;
    }

    // Expression may be an error; do not immediately propagate because a default might be provided
    const Value& expression = args[0];

    // Determine if we have a default value
    // If we have an odd number of arguments (excluding expression), the last one is default
    // Total args = 1 (expression) + pairs + [default]
    // So if (total - 1) is odd, we have a default
    bool hasDefault = ((args.size() - 1) % 2) == 1;
    Value defaultValue = hasDefault ? args[args.size() - 1] : Value::error(ErrorType::NA_ERROR);

    // Number of value/result pairs
    size_t numPairs = (args.size() - 1) / 2;

    // Check each value/result pair
    for (size_t i = 0; i < numPairs; ++i) {
        const Value& testValue = args[1 + i * 2];        // value to compare
        const Value& resultValue = args[1 + i * 2 + 1];  // result if match

        // Compare expression with testValue
        // Use exact comparison like Excel SWITCH
        bool match = false;

        if (expression.getType() == testValue.getType()) {
            switch (expression.getType()) {
                case ValueType::NUMBER:
                    match = (expression.asNumber() == testValue.asNumber());
                    break;
                case ValueType::TEXT:
                    match = (expression.asText() == testValue.asText());
                    break;
                case ValueType::BOOLEAN:
                    match = (expression.asBoolean() == testValue.asBoolean());
                    break;
                case ValueType::DATE:
                    match = (expression.asDate() == testValue.asDate());
                    break;
                case ValueType::ERROR:
                    match = (expression.asError() == testValue.asError());
                    break;
                case ValueType::ARRAY:
                    // Arrays not supported for comparison in SWITCH
                    match = false;
                    break;
                case ValueType::EMPTY:
                    // Both are empty, so they match
                    match = true;
                    break;
            }
        }

        if (match) {
            return resultValue;
        }
    }

    // No match found
    if (expression.isError()) {
        // If expression was an error, return default if provided, otherwise propagate the error
        return hasDefault ? defaultValue : expression;
    }
    return defaultValue;
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
