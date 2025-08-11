#include <string>
#include <sstream>
#include <algorithm>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

static std::string normalizeBinary(const std::string& input) {
    // Strip whitespace
    std::string s;
    for (char c : input) {
        if (!std::isspace(static_cast<unsigned char>(c))) s.push_back(c);
    }
    // Validate characters
    for (char c : s) {
        if (c != '0' && c != '1' && c != '-') {
            throw std::runtime_error("Invalid binary input");
        }
    }
    return s;
}

Value bin2oct(const std::vector<Value>& args, const Context& context) {
    return templates::baseConversionFunction(
        args, context, "BIN2OCT", [](const std::string& bin_str_in) -> std::string {
            std::string bin = normalizeBinary(bin_str_in);

            bool is_negative = false;
            if (!bin.empty() && bin[0] == '-') {
                is_negative = true;
                bin = bin.substr(1);
            }
            if (bin.empty()) {
                throw std::runtime_error("Invalid binary input");
            }

            // Limit to 10 bits as per Excel specification for binary inputs
            if (bin.length() > 10) {
                throw std::runtime_error("Binary length out of range");
            }

            // Parse as unsigned
            unsigned long long value = 0;
            for (char c : bin) {
                value = (value << 1) | (c == '1' ? 1ULL : 0ULL);
            }

            // For negative, interpret as two's complement with 10 bits
            if (is_negative) {
                // Excel encodes negatives in two's complement for given width, but inputs with '-' are
                // not standard. For interoperability, map -b to two's complement within 10 bits.
                if (value == 0) {
                    // -0 is 0
                } else {
                    unsigned long long width = 1ULL << 10; // 10 bits
                    if (value > width - 1) throw std::runtime_error("Binary length out of range");
                    value = (width - value);
                }
            }

            std::stringstream ss;
            ss << std::oct << std::uppercase << value;
            std::string result = ss.str();
            if (result.length() > 10) {
                throw std::runtime_error("Result too long");
            }
            return result;
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


