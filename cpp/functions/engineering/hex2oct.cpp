#include <string>
#include <sstream>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

static unsigned long long parseHex(const std::string& s) {
    unsigned long long value = 0;
    for (char c : s) {
        int digit = 0;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'A' && c <= 'F') digit = 10 + (c - 'A');
        else if (c >= 'a' && c <= 'f') digit = 10 + (c - 'a');
        else throw std::runtime_error("Invalid hex input");
        value = (value << 4) + static_cast<unsigned long long>(digit);
    }
    return value;
}

Value hex2oct(const std::vector<Value>& args, const Context& context) {
    return templates::baseConversionFunction(
        args, context, "HEX2OCT", [](const std::string& hex_in) -> std::string {
            std::string s;
            for (char c : hex_in) {
                if (!std::isspace(static_cast<unsigned char>(c))) s.push_back(c);
            }
            if (s.empty()) throw std::runtime_error("Invalid hex input");

            bool is_negative = false;
            if (s[0] == '-') {
                is_negative = true;
                s = s.substr(1);
            }

            unsigned long long value = parseHex(s);

            // Excel limits OCT output to 10 chars, which is up to 30 bits
            if (value > ((1ULL << 30) - 1ULL)) throw std::runtime_error("Result too long");

            if (is_negative) {
                unsigned long long width = 1ULL << 30;
                if (value == 0) {
                    // -0 -> 0
                } else {
                    value = width - value;
                }
            }

            std::stringstream ss;
            ss << std::oct << std::uppercase << value;
            std::string result = ss.str();
            if (result.length() > 10) throw std::runtime_error("Result too long");
            return result;
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


