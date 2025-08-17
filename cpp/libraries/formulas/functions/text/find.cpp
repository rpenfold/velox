#include <algorithm>
#include <cctype>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Finds one text string within another (case-sensitive)
 * @ingroup text
 * @param find_text Substring to find
 * @param within_text Text to search within
 * @param start_num Starting position (optional)
 * @code
 * FIND("lo", "Hello") -> 4
 * @endcode
 */
Value find(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // FIND requires 2-3 arguments
    auto error = utils::validateMinArgs(args, 2, "FIND");
    if (!error.isEmpty()) {
        return error;
    }

    if (args.size() > 3) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::string find_text = args[0].toString();
    std::string within_text = args[1].toString();

    // If find_text is empty, return 1 (Excel behavior)
    if (find_text.empty()) {
        return Value(1.0);
    }

    // If within_text is empty and find_text is not empty, return error
    if (within_text.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Default start position is 1 (1-based indexing)
    int start_num = 1;

    if (args.size() == 3) {
        if (!args[2].isNumber()) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        start_num = static_cast<int>(args[2].asNumber());

        // If start_num is less than 1, return error
        if (start_num < 1) {
            return Value::error(ErrorType::VALUE_ERROR);
        }

        // If start_num is greater than within_text length, return error
        if (start_num > static_cast<int>(within_text.length())) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    }

    // Convert to 0-based indexing for std::string::find
    size_t start_pos = static_cast<size_t>(start_num - 1);

    // Find the substring starting from start_pos
    // If start_pos is at the beginning of find_text, we need to skip it
    size_t found_pos = within_text.find(find_text, start_pos);

    // If we found the text at the start position, we need to find the next occurrence
    if (found_pos == start_pos && find_text.length() == 1) {
        found_pos = within_text.find(find_text, start_pos + 1);
    }
    // If not found, return #VALUE! error
    if (found_pos == std::string::npos) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Return 1-based position
    return Value(static_cast<double>(found_pos + 1));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula