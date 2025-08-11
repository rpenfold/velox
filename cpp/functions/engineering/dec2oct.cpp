#include <sstream>
#include <string>
#include <iomanip>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value dec2oct(const std::vector<Value>& args, const Context& context) {
    return templates::decimalToBaseFunction(
        args, context, "DEC2OCT", [](long long value, int places) -> std::string {
            // Excel supports -536870912 to 536870911 for OCT
            if (value < -536870912LL || value > 536870911LL) {
                throw std::runtime_error("Number out of range");
            }

            std::stringstream ss;
            if (value >= 0) {
                // Positive: standard octal
                ss << std::oct << std::uppercase << value;
            } else {
                // Negative: two's complement using 30 bits shown as octal up to 10 chars
                // 30 bits covers +/- 536,870,911 range
                unsigned long long unsigned_val = (1ULL << 30) + value;
                ss << std::oct << std::uppercase << unsigned_val;
            }

            std::string result = ss.str();

            if (places > 0 && result.length() < static_cast<size_t>(places)) {
                result = std::string(places - result.length(), '0') + result;
            }

            if (result.length() > 10) {
                throw std::runtime_error("Result too long");
            }

            return result;
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


