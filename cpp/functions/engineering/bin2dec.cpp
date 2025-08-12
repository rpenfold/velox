#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Converts a binary number to decimal
 * @ingroup engineering
 * @param number Binary string (up to 10 bits; two's complement for negatives)
 * @code
 * BIN2DEC("1010") -> 10
 * @endcode
 */
Value bin2dec(const std::vector<Value>& args, const Context& context) {
    return templates::baseConversionFunction(
            args, context, "BIN2DEC", [](const std::string& bin_str) -> double {
                std::string bin = bin_str;

                // Remove any whitespace
                bin.erase(std::remove_if(bin.begin(), bin.end(), ::isspace), bin.end());

                // Validate binary string (only 0 and 1 allowed)
                for (char c : bin) {
                    if (c != '0' && c != '1') {
                        throw std::runtime_error("Invalid binary character");
                    }
                }

                // Check length (Excel supports up to 10 binary digits)
                if (bin.length() > 10) {
                    throw std::runtime_error("Binary number too long");
                }

                if (bin.empty()) {
                    return 0.0;
                }

                try {
                    // Convert using standard library
                    unsigned long long result = std::stoull(bin, nullptr, 2);

                    // Handle negative numbers (two's complement for 10-bit signed integers)
                    if (bin.length() == 10 && bin[0] == '1') {
                        // This represents a negative number in two's complement
                        // For 10-bit: if MSB is 1, subtract 2^10 = 1024
                        return static_cast<double>(static_cast<long long>(result) - 1024);
                    }

                    return static_cast<double>(result);
                } catch (const std::exception&) {
                    throw std::runtime_error("Invalid binary number");
                }
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula