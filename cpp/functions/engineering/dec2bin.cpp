#include <bitset>
#include <string>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value dec2bin(const std::vector<Value>& args, const Context& context) {
    return templates::decimalToBaseFunction(args, context, "DEC2BIN",
        [](long long value, int places) -> std::string {
            // Validate range (Excel supports -512 to 511 for binary representation)
            if (value < -512 || value > 511) {
                throw std::runtime_error("Number out of range");
            }
            
            std::string result;
            
            if (value >= 0) {
                // Positive number - simple binary conversion
                if (value == 0) {
                    result = "0";
                } else {
                    result = std::bitset<32>(value).to_string();
                    // Remove leading zeros
                    size_t first_one = result.find('1');
                    if (first_one != std::string::npos) {
                        result = result.substr(first_one);
                    }
                }
            } else {
                // Negative number - use two's complement representation (10-bit)
                std::bitset<10> bits(static_cast<unsigned int>(value + 1024));
                result = bits.to_string();
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