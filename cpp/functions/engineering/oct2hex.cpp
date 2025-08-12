#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Converts an octal number to hexadecimal
 * @ingroup engineering
 * @param number Octal string (up to 10 digits)
 * @code
 * OCT2HEX("377") -> "FF"
 * @endcode
 */
Value oct2hex(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "OCT2HEX");
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert to text string for processing
    std::string oct_str = args[0].toString();

    // Remove any whitespace
    oct_str.erase(std::remove_if(oct_str.begin(), oct_str.end(), ::isspace), oct_str.end());

    // Validate octal string (only 0-7 allowed)
    for (char c : oct_str) {
        if (c < '0' || c > '7') {
            return Value::error(ErrorType::NUM_ERROR);
        }
    }

    // Check length (Excel supports up to 10 octal digits)
    if (oct_str.length() > 10 || oct_str.empty()) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    try {
        // Convert octal to decimal first
        unsigned long long decimal_result = std::stoull(oct_str, nullptr, 8);

        // Handle two's complement for large octal values (negative representation)
        long long signed_result;
        // For 10-digit octal, check if it represents a negative number
        if (oct_str.length() == 10 && oct_str[0] >= '4') {
            // This might represent a negative number in Excel's system
            signed_result = static_cast<long long>(decimal_result) - (1LL << 30);
        } else {
            signed_result = static_cast<long long>(decimal_result);
        }

        // Convert to hex string
        std::string hex_result;
        if (signed_result == 0) {
            hex_result = "0";
        } else if (signed_result > 0) {
            // Positive number
            std::stringstream ss;
            ss << std::uppercase << std::hex << signed_result;
            hex_result = ss.str();
        } else {
            // Negative number - use appropriate unsigned representation
            unsigned long long unsigned_val = static_cast<unsigned long long>(signed_result + (1LL << 40));
            
            std::stringstream ss;
            ss << std::uppercase << std::hex << unsigned_val;
            hex_result = ss.str();
        }

        return Value(hex_result);
    } catch (const std::exception&) {
        return Value::error(ErrorType::NUM_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
