#include "xl-formula/functions.h"
#include <regex>
#include <string>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the real coefficient of a complex number
 * @ingroup engineering
 * @param inumber Complex number as text
 * @code
 * IMREAL("3-4i") -> 3
 * @endcode
 */
Value imreal(const std::vector<Value>& args, const Context& context) {
    (void)context;

    auto validation = utils::validateArgCount(args, 1, "IMREAL");
    if (!validation.isEmpty()) {
        return validation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Get the complex number as text
    std::string complex_str = args[0].toString();
    
    // Remove spaces
    complex_str.erase(std::remove_if(complex_str.begin(), complex_str.end(), ::isspace), complex_str.end());

    if (complex_str.empty()) {
        return Value::error(ErrorType::NUM_ERROR);
    }

    try {
        // Pattern to match complex numbers: optional real part + optional imaginary part
        // Supports formats like: "3", "4i", "3+4i", "5-2j", "-3+4i", etc.
        std::regex complex_regex(R"(^([+-]?[0-9]*\.?[0-9]*)?([+-]?[0-9]*\.?[0-9]*)[ij]?$)");
        std::smatch matches;

        // First, check if it's a pure real number (no i or j)
        if (complex_str.find('i') == std::string::npos && complex_str.find('j') == std::string::npos) {
            // Pure real number
            try {
                double real_value = std::stod(complex_str);
                return Value(real_value);
            } catch (const std::exception&) {
                return Value::error(ErrorType::NUM_ERROR);
            }
        }

        // Check if it's a pure imaginary number (starts with i or j, or just coefficient+i/j)
        if ((complex_str[0] == 'i' || complex_str[0] == 'j') && complex_str.length() == 1) {
            // Just "i" or "j" means coefficient is 1, real part is 0
            return Value(0.0);
        }
        
        if (complex_str.back() == 'i' || complex_str.back() == 'j') {
            // Has imaginary unit at the end
            std::string coeff_part = complex_str.substr(0, complex_str.length() - 1);
            
            // Check if there's a + or - sign that separates real and imaginary parts
            size_t plus_pos = coeff_part.find_last_of('+');
            size_t minus_pos = coeff_part.find_last_of('-');
            
            // Find the position of the last +/- that's not at the beginning
            size_t separator_pos = std::string::npos;
            if (plus_pos != std::string::npos && plus_pos > 0) {
                separator_pos = plus_pos;
            }
            if (minus_pos != std::string::npos && minus_pos > 0) {
                if (separator_pos == std::string::npos || minus_pos > separator_pos) {
                    separator_pos = minus_pos;
                }
            }
            
            if (separator_pos != std::string::npos) {
                // Has both real and imaginary parts
                std::string real_part = coeff_part.substr(0, separator_pos);
                if (real_part.empty()) {
                    return Value(0.0);  // No real part
                }
                try {
                    double real_value = std::stod(real_part);
                    return Value(real_value);
                } catch (const std::exception&) {
                    return Value::error(ErrorType::NUM_ERROR);
                }
            } else {
                // Pure imaginary number like "3i" or "-4j"
                return Value(0.0);
            }
        }

        return Value::error(ErrorType::NUM_ERROR);
    } catch (const std::exception&) {
        return Value::error(ErrorType::NUM_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
