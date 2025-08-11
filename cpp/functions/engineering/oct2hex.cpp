#include <string>
#include <sstream>
#include <cctype>
#include <iomanip>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

static unsigned long long parseOct(const std::string& s) {
    unsigned long long value = 0;
    for (char c : s) {
        if (c < '0' || c > '7') throw std::runtime_error("Invalid octal input");
        value = (value << 3) + static_cast<unsigned long long>(c - '0');
    }
    return value;
}

Value oct2hex(const std::vector<Value>& args, const Context& context) {
    return templates::baseConversionFunction(
        args, context, "OCT2HEX", [](const std::string& oct_in) -> std::string {
            std::string s;
            for (char c : oct_in) {
                if (!std::isspace(static_cast<unsigned char>(c))) s.push_back(c);
            }
            if (s.empty()) throw std::runtime_error("Invalid octal input");

            bool is_negative = false;
            if (s[0] == '-') {
                is_negative = true;
                s = s.substr(1);
            }

            unsigned long long value = parseOct(s);
            // Excel HEX output limited to 10 chars => up to 40 bits
            if (value > ((1ULL << 40) - 1ULL)) throw std::runtime_error("Result too long");

            if (is_negative) {
                unsigned long long width = 1ULL << 40;
                if (value == 0) {
                } else {
                    value = width - value;
                }
            }

            std::stringstream ss;
            ss << std::hex << std::uppercase << value;
            std::string result = ss.str();
            if (result.length() > 10) throw std::runtime_error("Result too long");
            return result;
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


