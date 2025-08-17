#include <bitset>
#include <string>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Converts a decimal number to octal
 * @ingroup engineering
 * @param number Decimal number (range per Excel limits)
 * @param places Number of characters to use (optional)
 * @code
 * DEC2OCT(8) -> "10"
 * @endcode
 */
Value dec2oct(const std::vector<Value>& args, const Context& context) {
    return templates::decimalToBaseFunction(
            args, context, "DEC2OCT", [](long long value, int places) -> std::string {
                // Validate range (Excel supports -536870912 to 536870911 for octal)
                if (value < -536870912LL || value > 536870911LL) {
                    throw std::runtime_error("Number out of range");
                }

                std::string result;

                if (value >= 0) {
                    // Positive number - simple octal conversion
                    if (value == 0) {
                        result = "0";
                    } else {
                        // Convert to octal
                        std::string temp;
                        long long num = value;
                        while (num > 0) {
                            temp = std::to_string(num % 8) + temp;
                            num /= 8;
                        }
                        result = temp;
                    }
                } else {
                    // Negative number - use two's complement representation (30-bit for octal)
                    // Excel uses 30-bit signed integers for octal
                    unsigned long long unsigned_val =
                            static_cast<unsigned long long>(value + (1LL << 30));

                    // Convert to octal
                    std::string temp;
                    if (unsigned_val == 0) {
                        temp = "0";
                    } else {
                        while (unsigned_val > 0) {
                            temp = std::to_string(unsigned_val % 8) + temp;
                            unsigned_val /= 8;
                        }
                    }
                    result = temp;
                }

                // Apply padding if places is specified and greater than current length
                if (places > 0 && result.length() < static_cast<size_t>(places)) {
                    result = std::string(places - result.length(), '0') + result;
                }

                // Validate maximum length (10 characters for Excel compatibility)
                if (result.length() > 10) {
                    throw std::runtime_error("Result too long");
                }

                return result;
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
