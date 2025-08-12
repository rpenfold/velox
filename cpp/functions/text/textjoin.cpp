#include <sstream>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief TEXTJOIN function - joins text values with a delimiter
 * @param args Function arguments (delimiter, ignore_empty, text1, text2, ...)
 * @param context Evaluation context (unused for TEXTJOIN)
 * @return Joined text string
 *
 * Excel syntax: TEXTJOIN(delimiter, ignore_empty, text1, [text2], ...)
 * - delimiter: Text to use as separator
 * - ignore_empty: TRUE to ignore empty values, FALSE to include them
 * - text1, text2, ...: Values to join (minimum 1 required)
 */
Value textjoin(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // Validate minimum argument count (delimiter, ignore_empty, text1)
    auto minValidation = utils::validateMinArgs(args, 3, "TEXTJOIN");
    if (!minValidation.isEmpty()) {
        return minValidation;
    }

    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // First argument: delimiter (convert to text)
    std::string delimiter = args[0].toString();

    // Second argument: ignore_empty (convert to boolean)
    bool ignoreEmpty = false;
    if (args[1].isBoolean()) {
        ignoreEmpty = args[1].asBoolean();
    } else {
        // Try to convert to number, then to boolean (0 = false, non-zero = true)
        auto numResult = utils::toNumberSafe(args[1], "TEXTJOIN");
        if (numResult.isError()) {
            return numResult;
        }
        ignoreEmpty = (numResult.asNumber() != 0.0);
    }

    // Collect all text values to join
    std::vector<std::string> textValues;

    for (size_t i = 2; i < args.size(); ++i) {
        std::string text = args[i].toString();

        // If ignore_empty is true, skip empty strings
        if (ignoreEmpty && text.empty()) {
            continue;
        }

        textValues.push_back(text);
    }

    // Join the text values with the delimiter
    std::string result;
    for (size_t i = 0; i < textValues.size(); ++i) {
        if (i > 0) {
            result += delimiter;
        }
        result += textValues[i];
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
