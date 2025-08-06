#include <algorithm>
#include <cctype>
#include <sstream>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief VALUE function - converts a text string to a number
 * @param args Function arguments (text)
 * @param context Evaluation context (unused for VALUE)
 * @return Numeric value
 */
Value value(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // VALUE requires exactly 1 argument
    auto error = utils::validateArgCount(args, 1, "VALUE");
    if (!error.isEmpty()) {
        return error;
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::string text = args[0].toString();
    
    // Remove leading and trailing whitespace
    text.erase(0, text.find_first_not_of(" \t\n\r"));
    text.erase(text.find_last_not_of(" \t\n\r") + 1);
    
    if (text.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    // Handle percentage values
    bool is_percentage = false;
    if (text.back() == '%') {
        is_percentage = true;
        text.pop_back();
    }
    
    // Handle currency symbols
    if (text.front() == '$') {
        text.erase(0, 1);
    }
    
    // Handle negative currency (e.g., -$123.45)
    if (text.front() == '-' && text.length() > 1 && text[1] == '$') {
        text.erase(1, 1); // Remove the $ but keep the minus sign
    }
    
    // Handle boolean text values
    if (text == "TRUE" || text == "true") {
        return Value(1.0);
    }
    if (text == "FALSE" || text == "false") {
        return Value(0.0);
    }
    // Try to convert to double
    std::istringstream iss(text);
    double result;
    
    if (iss >> result) {
        // Check if there are any remaining characters (indicating invalid input)
        char remaining;
        if (iss >> remaining) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        // If it was a percentage, divide by 100
        if (is_percentage) {
            result /= 100.0;
        }
        return Value(result);
    }
    
    // If conversion failed, return error
    return Value::error(ErrorType::VALUE_ERROR);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula