#include <string>
#include <sstream>
#include <cctype>
#include <bitset>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

static std::string trimString(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (!std::isspace(static_cast<unsigned char>(c))) out.push_back(c);
    }
    return out;
}

Value oct2bin(const std::vector<Value>& args, const Context& context) {
    return templates::baseConversionFunction(
        args, context, "OCT2BIN", [](const std::string& oct_in) -> std::string {
            std::string oct = trimString(oct_in);
            if (oct.empty()) throw std::runtime_error("Invalid octal input");

            bool is_negative = false;
            if (oct[0] == '-') {
                is_negative = true;
                oct = oct.substr(1);
            }

            unsigned long long value = 0;
            for (char c : oct) {
                if (c < '0' || c > '7') throw std::runtime_error("Invalid octal input");
                value = (value << 3) + static_cast<unsigned long long>(c - '0');
            }

            // Excel limits binary output to 10 characters
            if (value > 1023ULL) throw std::runtime_error("Result too long");

            std::string bin;
            if (!is_negative) {
                if (value == 0) {
                    bin = "0";
                } else {
                    std::bitset<10> bits(value);
                    bin = bits.to_string();
                    // Strip leading zeros
                    size_t first_one = bin.find('1');
                    if (first_one != std::string::npos) bin = bin.substr(first_one);
                }
            } else {
                // two's complement in 10 bits
                unsigned long long width = 1ULL << 10;
                if (value == 0) {
                    bin = "0";
                } else {
                    unsigned long long tc = width - value;
                    std::bitset<10> bits(tc);
                    bin = bits.to_string();
                }
            }

            if (bin.length() > 10) throw std::runtime_error("Result too long");
            return bin;
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


