#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Converts a hexadecimal number to octal
 * @ingroup engineering
 * @param number Hex string (up to 10 hex digits)
 * @code
 * HEX2OCT("FF") -> "377"
 * @endcode
 */
Value hex2oct(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "HEX2OCT");
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert to text string for processing
    std::string hex_str = args[0].toString();

    // Remove any whitespace
    hex_str.erase(std::remove_if(hex_str.begin(), hex_str.end(), ::isspace), hex_str.end());

    // Convert to uppercase for consistency
    std::transform(hex_str.begin(), hex_str.end(), hex_str.begin(), ::toupper);

    // Validate hex string (only 0-9, A-F allowed)
    for (char c : hex_str) {
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
            return Value::error(ErrorType::NUM_ERROR);
        }
    }

    // Check length (Excel supports up to 10 hex digits)
    if (hex_str.length() > 10 || hex_str.empty()) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    try {
        // Convert hex to decimal first
        unsigned long long decimal_result = std::stoull(hex_str, nullptr, 16);

        // Handle two's complement for negative numbers based on hex length
        long long signed_result;
        if (hex_str.length() == 10) {
            // 40-bit values: treat as two's complement for negative numbers
            if (decimal_result > 0x7FFFFFFFFFULL) {
                signed_result = static_cast<long long>(decimal_result - 0x10000000000ULL);
            } else {
                signed_result = static_cast<long long>(decimal_result);
            }
        } else if (hex_str.length() == 8) {
            // 32-bit values: handle two's complement
            if (decimal_result > 0x7FFFFFFFULL) {
                signed_result = static_cast<long long>(decimal_result - 0x100000000ULL);
            } else {
                signed_result = static_cast<long long>(decimal_result);
            }
        } else {
            signed_result = static_cast<long long>(decimal_result);
        }

        // Convert to octal string
        std::string octal_result;
        if (signed_result == 0) {
            octal_result = "0";
        } else if (signed_result > 0) {
            // Positive number
            std::string temp;
            long long num = signed_result;
            while (num > 0) {
                temp = std::to_string(num % 8) + temp;
                num /= 8;
            }
            octal_result = temp;
        } else {
            // Negative number - use appropriate representation for round-trip compatibility
            unsigned long long unsigned_val =
                    static_cast<unsigned long long>(signed_result + (1LL << 30));

            std::string temp;
            if (unsigned_val == 0) {
                temp = "0";
            } else {
                while (unsigned_val > 0) {
                    temp = std::to_string(unsigned_val % 8) + temp;
                    unsigned_val /= 8;
                }
            }
            octal_result = temp;
        }

        return Value(octal_result);
    } catch (const std::exception&) {
        return Value::error(ErrorType::NUM_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
