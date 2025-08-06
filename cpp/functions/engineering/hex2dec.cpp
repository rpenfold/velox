#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value hex2dec(const std::vector<Value>& args, const Context& context) {
    return templates::baseConversionFunction(args, context, "HEX2DEC",
        [](const std::string& hex_str) -> double {
            std::string hex = hex_str;
            
            // Remove any whitespace
            hex.erase(std::remove_if(hex.begin(), hex.end(), ::isspace), hex.end());
            
            // Convert to uppercase for consistency
            std::transform(hex.begin(), hex.end(), hex.begin(), ::toupper);
            
            // Validate hex string (only 0-9, A-F allowed)
            for (char c : hex) {
                if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
                    throw std::runtime_error("Invalid hexadecimal character");
                }
            }
            
            // Check length (Excel supports up to 10 hex digits)
            if (hex.length() > 10) {
                throw std::runtime_error("Hexadecimal number too long");
            }
            
            if (hex.empty()) {
                return 0.0;
            }
            
            try {
                // Convert using standard library
                unsigned long long result = std::stoull(hex, nullptr, 16);
                
                // Handle two's complement for negative numbers  
                if (hex.length() == 10) {
                    // 40-bit values: treat as two's complement for negative numbers
                    if (result > 0x7FFFFFFFFFULL) {
                        // This is a negative number in 40-bit two's complement
                        return static_cast<double>(static_cast<long long>(result - 0x10000000000ULL));
                    }
                } else if (hex.length() == 8) {
                    // 32-bit values: handle two's complement
                    if (result > 0x7FFFFFFFULL) {
                        // This is a negative number in 32-bit two's complement  
                        return static_cast<double>(static_cast<long long>(result - 0x100000000ULL));
                    }
                }
                
                return static_cast<double>(result);
            } catch (const std::exception&) {
                throw std::runtime_error("Invalid hexadecimal number");
            }
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula