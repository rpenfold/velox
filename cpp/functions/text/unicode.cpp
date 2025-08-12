#include <string>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief UNICODE function - returns Unicode code point for first character
 * @param args Function arguments (text)
 * @param context Evaluation context (unused for UNICODE)
 * @return Unicode code point of the first character
 *
 * Excel behavior:
 * - Returns Unicode code point (1-1114111) of the first character in text
 * - Returns error for empty string
 * - Only examines the first character if multiple characters present
 */
Value unicode_function(const std::vector<Value>& args, const Context& context) {
    (void)context;

    auto countValidation = utils::validateArgCount(args, 1, "UNICODE");
    if (!countValidation.isEmpty()) {
        return countValidation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::string text = args[0].toString();

    if (text.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Decode first UTF-8 character to get Unicode code point
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(text.c_str());
    int codePoint = 0;

    if (bytes[0] <= 0x7F) {
        // ASCII (1 byte)
        codePoint = bytes[0];
    } else if ((bytes[0] & 0xE0) == 0xC0) {
        // 2-byte UTF-8
        if (text.length() < 2) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        codePoint = ((bytes[0] & 0x1F) << 6) | (bytes[1] & 0x3F);
    } else if ((bytes[0] & 0xF0) == 0xE0) {
        // 3-byte UTF-8
        if (text.length() < 3) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        codePoint = ((bytes[0] & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
    } else if ((bytes[0] & 0xF8) == 0xF0) {
        // 4-byte UTF-8
        if (text.length() < 4) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        codePoint = ((bytes[0] & 0x07) << 18) | ((bytes[1] & 0x3F) << 12) |
                    ((bytes[2] & 0x3F) << 6) | (bytes[3] & 0x3F);
    } else {
        // Invalid UTF-8
        return Value::error(ErrorType::VALUE_ERROR);
    }

    return Value(static_cast<double>(codePoint));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
