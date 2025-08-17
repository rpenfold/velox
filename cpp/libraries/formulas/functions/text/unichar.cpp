#include <string>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief UNICHAR function - returns Unicode character for given code point
 * @param args Function arguments (code_point)
 * @param context Evaluation context (unused for UNICHAR)
 * @return Unicode character corresponding to the code point
 *
 * Excel behavior:
 * - Returns Unicode character for code points 1-1114111 (0x10FFFF)
 * - Returns error for invalid code points (0 or > 1114111)
 */
Value unichar(const std::vector<Value>& args, const Context& context) {
    (void)context;

    auto countValidation = utils::validateArgCount(args, 1, "UNICHAR");
    if (!countValidation.isEmpty()) {
        return countValidation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    auto numResult = utils::toNumberSafe(args[0], "UNICHAR");
    if (numResult.isError()) {
        return numResult;
    }

    double codePointDouble = numResult.asNumber();

    // Truncate to integer
    int codePoint = static_cast<int>(codePointDouble);

    // Validate Unicode code point range (1 to 1114111)
    if (codePoint <= 0 || codePoint > 0x10FFFF) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Convert code point to UTF-8 string
    std::string result;

    if (codePoint <= 0x7F) {
        // ASCII range (1 byte)
        result = static_cast<char>(codePoint);
    } else if (codePoint <= 0x7FF) {
        // 2-byte UTF-8
        result += static_cast<char>(0xC0 | (codePoint >> 6));
        result += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else if (codePoint <= 0xFFFF) {
        // 3-byte UTF-8
        result += static_cast<char>(0xE0 | (codePoint >> 12));
        result += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else {
        // 4-byte UTF-8
        result += static_cast<char>(0xF0 | (codePoint >> 18));
        result += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codePoint & 0x3F));
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
