#include "xl-formula/functions.h"
#include <regex>
#include <string>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the imaginary coefficient of a complex number
 * @ingroup engineering
 * @param inumber Complex number as text
 * @code
 * IMAGINARY("3-4i") -> -4
 * @endcode
 */
Value imaginary(const std::vector<Value>& args, const Context& context) {
    (void)context;

    auto validation = utils::validateArgCount(args, 1, "IMAGINARY");
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
        // First, check if it's a pure real number (no i or j)
        if (complex_str.find('i') == std::string::npos && complex_str.find('j') == std::string::npos) {
            // Pure real number, imaginary part is 0
            return Value(0.0);
        }

        // Check if it's just "i" or "j"
        if ((complex_str == "i" || complex_str == "j")) {
            return Value(1.0);
        }
        if ((complex_str == "-i" || complex_str == "-j")) {
            return Value(-1.0);
        }
        if ((complex_str == "+i" || complex_str == "+j")) {
            return Value(1.0);
        }

        // Has imaginary unit
        if (complex_str.back() == 'i' || complex_str.back() == 'j') {
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
                // Has both real and imaginary parts like "3+4i" or "5-2j"
                std::string imag_part = coeff_part.substr(separator_pos);
                if (imag_part.empty() || imag_part == "+") {
                    return Value(1.0);  // "+i" case
                }
                if (imag_part == "-") {
                    return Value(-1.0);  // "-i" case
                }
                try {
                    double imag_value = std::stod(imag_part);
                    return Value(imag_value);
                } catch (const std::exception&) {
                    return Value::error(ErrorType::NUM_ERROR);
                }
            } else {
                // Pure imaginary number like "3i" or "-4j"
                if (coeff_part.empty()) {
                    return Value(1.0);  // Just "i" or "j"
                }
                if (coeff_part == "+") {
                    return Value(1.0);  // "+i" or "+j"
                }
                if (coeff_part == "-") {
                    return Value(-1.0);  // "-i" or "-j"
                }
                try {
                    double imag_value = std::stod(coeff_part);
                    return Value(imag_value);
                } catch (const std::exception&) {
                    return Value::error(ErrorType::NUM_ERROR);
                }
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
