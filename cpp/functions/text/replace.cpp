#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief REPLACE function - replaces part of a text string
 * @param args Function arguments (old_text, start_num, num_chars, new_text)
 * @param context Evaluation context (unused for REPLACE)
 * @return Text with the specified characters replaced
 */
Value replace(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // REPLACE requires exactly 4 arguments
    auto error = utils::validateArgCount(args, 4, "REPLACE");
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

    std::string old_text = args[0].toString();
    int start_num = static_cast<int>(args[1].asNumber());
    int num_chars = static_cast<int>(args[2].asNumber());
    std::string new_text = args[3].toString();
    
    // If start_num is less than 1, return error
    if (start_num < 1) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    // If num_chars is negative, return error
    if (num_chars < 0) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    // Convert to 0-based indexing
    size_t start_pos = static_cast<size_t>(start_num - 1);
    
    // If start_pos is beyond the text length, append new_text to the end
    if (start_pos >= old_text.length()) {
        return Value(old_text + new_text);
    }
    
    // Calculate the actual number of characters to replace
    size_t available_chars = old_text.length() - start_pos;
    size_t chars_to_replace = std::min(static_cast<size_t>(num_chars), available_chars);
    
    // Build the result string
    std::string result = old_text.substr(0, start_pos);
    result += new_text;
    result += old_text.substr(start_pos + chars_to_replace);
    
    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula