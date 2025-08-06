#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief MID function - returns characters from the middle of text
 * @param args Function arguments (text, start_num, num_chars)
 * @param context Evaluation context (unused for MID)
 * @return Characters from the middle of text
 */
Value mid(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // MID requires exactly 3 arguments
    auto error = utils::validateArgCount(args, 3, "MID");
    if (!error.isEmpty()) {
        return error;
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Validate that start_num and num_chars are numbers
    if (!args[1].isNumber() || !args[2].isNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    std::string text = args[0].toString();
    int start_num = static_cast<int>(args[1].asNumber());
    int num_chars = static_cast<int>(args[2].asNumber());
    
    // If start_num is less than 1, return error
    if (start_num < 1) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    // If num_chars is negative, return empty string
    if (num_chars < 0) {
        return Value("");
    }
    
    // Convert to 0-based indexing
    size_t start_pos = static_cast<size_t>(start_num - 1);
    
    // If start_pos is beyond the text length, return empty string
    if (start_pos >= text.length()) {
        return Value("");
    }
    
    // Calculate the actual number of characters to extract
    size_t available_chars = text.length() - start_pos;
    size_t chars_to_extract = std::min(static_cast<size_t>(num_chars), available_chars);
    
    // Return the substring
    return Value(text.substr(start_pos, chars_to_extract));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula