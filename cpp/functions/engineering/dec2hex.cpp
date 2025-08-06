#include <iomanip>
#include <sstream>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value dec2hex(const std::vector<Value>& args, const Context& context) {
    return templates::decimalToBaseFunction(args, context, "DEC2HEX",
        [](long long value, int places) -> std::string {
            // Validate range (Excel supports -549755813888 to 549755813887)
            if (value < -549755813888LL || value > 549755813887LL) {
                throw std::runtime_error("Number out of range");
            }
            
            std::stringstream ss;
            
            if (value >= 0) {
                // Positive number
                ss << std::hex << std::uppercase << value;
            } else {
                // Negative number - use two's complement representation
                // Excel's behavior varies by value range
                if (value > -512) {
                    // Use 40-bit representation for small negative numbers (-1, -2, ..., -511)
                    unsigned long long unsigned_val = (1LL << 40) + value;
                    ss << std::hex << std::uppercase << unsigned_val;
                } else if (value >= -2147483648) {
                    // Use 32-bit representation for medium negative numbers (-512 to -2^31)
                    unsigned int unsigned_val = static_cast<unsigned int>(static_cast<int>(value));
                    ss << std::hex << std::uppercase << unsigned_val;
                } else {
                    // Use 40-bit representation for very large negative numbers
                    unsigned long long unsigned_val = (1LL << 40) + value;
                    ss << std::hex << std::uppercase << unsigned_val;
                }
            }
            
            std::string result = ss.str();
            
            // Apply padding if places is specified and greater than current length
            if (places > 0 && result.length() < static_cast<size_t>(places)) {
                result = std::string(places - result.length(), '0') + result;
            }
            
            // Validate maximum length (10 characters for 40-bit)
            if (result.length() > 10) {
                throw std::runtime_error("Result too long");
            }
            
            return result;
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula