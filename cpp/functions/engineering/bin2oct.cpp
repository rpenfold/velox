#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Converts a binary number to octal
 * @ingroup engineering
 * @param number Binary string (up to 10 bits)
 * @code
 * BIN2OCT("1111111111") -> "1777"
 * @endcode
 */
Value bin2oct(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "BIN2OCT");
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert to text string for processing
    std::string bin_str = args[0].toString();

    // Remove any whitespace
    bin_str.erase(std::remove_if(bin_str.begin(), bin_str.end(), ::isspace), bin_str.end());

    // Validate binary string (only 0 and 1 allowed)
    for (char c : bin_str) {
        if (c != '0' && c != '1') {
            return Value::error(ErrorType::NUM_ERROR);
        }
    }

    // Check length (Excel supports up to 10 binary digits)
    if (bin_str.length() > 10 || bin_str.empty()) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    try {
        // Convert binary to decimal first
        unsigned long long decimal_result = std::stoull(bin_str, nullptr, 2);

        // Handle negative numbers (two's complement for 10-bit signed integers)
        long long signed_result;
        if (bin_str.length() == 10 && bin_str[0] == '1') {
            // This represents a negative number in two's complement
            signed_result = static_cast<long long>(decimal_result) - 1024;
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
            // Negative number - use appropriate representation
            // Excel represents negative numbers in a way that maintains round-trip compatibility
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
