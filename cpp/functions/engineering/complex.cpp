#include "xl-formula/functions.h"
#include <sstream>
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Creates a complex number from real and imaginary parts
 * @ingroup engineering
 * @param real_num Real coefficient
 * @param i_num Imaginary coefficient
 * @param suffix Suffix for imaginary part (optional; i or j)
 * @code
 * COMPLEX(3, -4) -> "3-4i"
 * @endcode
 */
Value complex_function(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // Validate argument count (2 or 3 arguments)
    if (args.size() < 2 || args.size() > 3) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // Convert arguments to numbers
    auto realResult = utils::toNumberSafe(args[0], "COMPLEX");
    if (realResult.isError()) {
        return realResult;
    }
    double real_part = realResult.asNumber();

    auto imagResult = utils::toNumberSafe(args[1], "COMPLEX");
    if (imagResult.isError()) {
        return imagResult;
    }
    double imag_part = imagResult.asNumber();

    // Get suffix (default is "i")
    std::string suffix = "i";
    if (args.size() == 3) {
        suffix = args[2].toString();
        
        // Validate suffix (Excel only allows "i" or "j")
        if (suffix != "i" && suffix != "j") {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    }

    // Build complex number string
    std::stringstream ss;
    
    // Handle special cases
    if (real_part == 0.0 && imag_part == 0.0) {
        return Value("0");
    }
    
    if (imag_part == 0.0) {
        // Pure real number
        ss << real_part;
        return Value(ss.str());
    }
    
    if (real_part == 0.0) {
        // Pure imaginary number
        if (imag_part == 1.0) {
            ss << suffix;
        } else if (imag_part == -1.0) {
            ss << "-" << suffix;
        } else {
            ss << imag_part << suffix;
        }
        return Value(ss.str());
    }
    
    // Mixed real and imaginary
    ss << real_part;
    
    if (imag_part > 0.0) {
        if (imag_part == 1.0) {
            ss << "+" << suffix;
        } else {
            ss << "+" << imag_part << suffix;
        }
    } else {
        if (imag_part == -1.0) {
            ss << "-" << suffix;
        } else {
            ss << imag_part << suffix;  // imag_part is negative, so it includes the minus sign
        }
    }

    return Value(ss.str());
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
