#include <string>
#include <vector>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

// Basic ROMAN/ARABIC helpers (classic form only)
static std::string toRomanInt(long long num) {
    if (num <= 0)
        return "";
    struct Item {
        int val;
        const char* sym;
    };
    static const Item items[] = {{1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"},
                                 {90, "XC"},  {50, "L"},   {40, "XL"}, {10, "X"},   {9, "IX"},
                                 {5, "V"},    {4, "IV"},   {1, "I"}};
    std::string out;
    for (const auto& it : items) {
        while (num >= it.val) {
            out += it.sym;
            num -= it.val;
        }
    }
    return out;
}

static long long fromRoman(const std::string& s) {
    auto val = [](char c) {
        switch (c) {
            case 'I':
                return 1;
            case 'V':
                return 5;
            case 'X':
                return 10;
            case 'L':
                return 50;
            case 'C':
                return 100;
            case 'D':
                return 500;
            case 'M':
                return 1000;
            default:
                return 0;
        }
    };
    long long total = 0;
    long long prev = 0;
    for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
        int v = val(static_cast<char>(std::toupper(s[i])));
        if (v < prev)
            total -= v;
        else {
            total += v;
            prev = v;
        }
    }
    return total;
}

/**
 * @brief Converts an Arabic number to Roman numerals
 * @ingroup text
 * @param number Positive integer to convert to Roman numerals
 * @code
 * ROMAN(1987) -> "MCMLXXXVII"
 * @endcode
 */
Value roman(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 1, "ROMAN");
    if (!err.isEmpty())
        return err;
    auto n = utils::toNumberSafe(args[0], "ROMAN");
    if (n.isError())
        return n;
    long long v = static_cast<long long>(n.asNumber());
    if (v <= 0)
        return Value::error(ErrorType::NUM_ERROR);
    return Value(toRomanInt(v));
}

/**
 * @brief Converts Roman numerals to an Arabic number
 * @ingroup text
 * @param text Roman numeral string to convert
 * @code
 * ARABIC("IV") -> 4
 * @endcode
 */
Value arabic(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 1, "ARABIC");
    if (!err.isEmpty())
        return err;
    std::string s = args[0].toString();
    if (s.empty())
        return Value::error(ErrorType::VALUE_ERROR);
    return Value(static_cast<double>(fromRoman(s)));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
