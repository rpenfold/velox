#include <string>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the character specified by a code number (1-255)
 * @ingroup text
 * @param number Code number between 1 and 255
 * @code
 * CHAR(65) -> "A"
 * @endcode
 */
Value char_function(const std::vector<Value>& args, const Context& context) {
    (void)context;

    auto countValidation = utils::validateArgCount(args, 1, "CHAR");
    if (!countValidation.isEmpty()) {
        return countValidation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    auto numResult = utils::toNumberSafe(args[0], "CHAR");
    if (numResult.isError()) {
        return numResult;
    }

    double code = numResult.asNumber();
    // Truncate to integer
    int intCode = static_cast<int>(code);
    if (intCode <= 0 || intCode > 255) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    char c = static_cast<char>(static_cast<unsigned char>(intCode));
    std::string result(1, c);
    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
