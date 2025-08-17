#include <algorithm>
#include <bitset>
#include <cctype>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Converts an octal number to binary
 * @ingroup engineering
 * @param number Octal string (up to 10 digits)
 * @code
 * OCT2BIN("1777") -> "1111111111"
 * @endcode
 */
Value oct2bin(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 1, "OCT2BIN");
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

        // Check range for binary representation (-512 to 511, which maps to 0-1023 unsigned)
        if (decimal_result > 1023) {  // 2^10 - 1
            return Value::error(ErrorType::NUM_ERROR);
        }

        // Convert to binary string
        std::string binary_result;
        if (decimal_result == 0) {
            binary_result = "0";
        } else if (decimal_result <= 511) {
            // Positive number
            binary_result = std::bitset<32>(decimal_result).to_string();
            // Remove leading zeros
            size_t first_one = binary_result.find('1');
            if (first_one != std::string::npos) {
                binary_result = binary_result.substr(first_one);
            }
        } else {
            // Negative number in two's complement (10-bit)
            binary_result = std::bitset<10>(decimal_result).to_string();
        }

        return Value(binary_result);
    } catch (const std::exception&) {
        return Value::error(ErrorType::NUM_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula