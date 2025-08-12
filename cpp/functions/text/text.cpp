#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Formats a number as text according to a specified format
 * @ingroup text
 * @param value Number to format
 * @param format_text Format pattern (e.g., 0.00, 0%)
 * @code
 * TEXT(123.456, "0.00") -> "123.46"
 * @endcode
 */
Value text(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // TEXT requires exactly 2 arguments
    auto error = utils::validateArgCount(args, 2, "TEXT");
    if (!error.isEmpty()) {
        return error;
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // First argument should be a number or convertible to number
    if (!args[0].isNumber() && !args[0].canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    double value = args[0].isNumber() ? args[0].asNumber() : args[0].toNumber();
    std::string format_text = args[1].toString();

    // Basic format implementation - supports common Excel formats
    // This is a simplified version; full Excel format support would be much more complex

    if (format_text.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Convert format_text to uppercase for easier parsing
    std::string format_upper = format_text;
    std::transform(format_upper.begin(), format_upper.end(), format_upper.begin(), ::toupper);

    std::ostringstream oss;

    // Handle common format patterns
    if (format_upper == "0" || format_upper == "#") {
        // Integer format
        oss << std::fixed << std::setprecision(0) << value;
    } else if (format_upper == "0.00" || format_upper == "#.##") {
        // Decimal format
        oss << std::fixed << std::setprecision(2) << value;
    } else if (format_upper == "0%" || format_upper == "#%") {
        // Percentage format
        oss << std::fixed << std::setprecision(0) << (value * 100) << "%";
    } else if (format_upper == "0.00%" || format_upper == "#.##%") {
        // Percentage with decimals
        oss << std::fixed << std::setprecision(2) << (value * 100) << "%";
    } else if (format_upper == "$#,##0.00" || format_upper == "$0.00") {
        // Currency format
        oss << "$" << std::fixed << std::setprecision(2) << value;
    } else if (format_upper == "MM/DD/YYYY" || format_upper == "M/D/YYYY") {
        // Date format (simplified - assumes value is days since 1900)
        // This is a very basic implementation
        int days = static_cast<int>(value);
        int year = 1900 + (days / 365);
        int month = 1 + ((days % 365) / 30);
        int day = 1 + ((days % 365) % 30);
        oss << std::setfill('0') << std::setw(2) << month << "/" << std::setw(2) << day << "/"
            << std::setw(4) << year;
    } else {
        // Default format - just convert to string
        oss << std::fixed << std::setprecision(6) << value;
    }

    return Value(oss.str());
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula