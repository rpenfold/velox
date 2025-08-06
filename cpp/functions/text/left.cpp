#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief LEFT function - returns the leftmost characters from text
 * @param args Function arguments (text, [num_chars])
 * @param context Evaluation context (unused for LEFT)
 * @return Leftmost characters from text
 */
Value left(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // LEFT requires 1-2 arguments
    auto error = utils::validateMinArgs(args, 1, "LEFT");
    if (!error.isEmpty()) {
        return error;
    }

    if (args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::string text = args[0].toString();
    
    // Default to 1 character if num_chars not specified
    int num_chars = 1;
    
    if (args.size() == 2) {
        if (!args[1].isNumber()) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        num_chars = static_cast<int>(args[1].asNumber());
        
        // If num_chars is negative, return empty string
        if (num_chars < 0) {
            return Value("");
        }
    }

    // If num_chars is greater than text length, return entire text
    if (num_chars >= static_cast<int>(text.length())) {
        return Value(text);
    }

    // Return the leftmost num_chars characters
    return Value(text.substr(0, num_chars));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula