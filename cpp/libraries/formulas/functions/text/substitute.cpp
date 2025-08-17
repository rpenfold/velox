#include <algorithm>
#include <cctype>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Substitutes new text for old text
 * @ingroup text
 * @param text Original text
 * @param old_text Text to replace
 * @param new_text Replacement text
 * @param instance_num Which occurrence to replace (optional; default all)
 * @code
 * SUBSTITUTE("a,b,c", ",", ";") -> "a;b;c"
 * @endcode
 */
Value substitute(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // SUBSTITUTE requires 3-4 arguments
    auto error = utils::validateMinArgs(args, 3, "SUBSTITUTE");
    if (!error.isEmpty()) {
        return error;
    }

    if (args.size() > 4) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::string text = args[0].toString();
    std::string old_text = args[1].toString();
    std::string new_text = args[2].toString();

    // If old_text is empty, return original text
    if (old_text.empty()) {
        return Value(text);
    }

    // Default to replace all instances
    int instance_num = -1;

    if (args.size() == 4) {
        if (!args[3].isNumber()) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        instance_num = static_cast<int>(args[3].asNumber());

        // If instance_num is less than 1, return error
        if (instance_num < 1) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    }

    std::string result = text;

    if (instance_num == -1) {
        // Replace all instances
        size_t pos = 0;
        while ((pos = result.find(old_text, pos)) != std::string::npos) {
            result.replace(pos, old_text.length(), new_text);
            pos += new_text.length();
        }
    } else {
        // Replace specific instance
        size_t pos = 0;
        int current_instance = 0;

        while ((pos = result.find(old_text, pos)) != std::string::npos) {
            current_instance++;
            if (current_instance == instance_num) {
                result.replace(pos, old_text.length(), new_text);
                break;
            }
            pos += old_text.length();
        }

        // If the specified instance wasn't found, return original text
        if (current_instance < instance_num) {
            return Value(text);
        }
    }

    return Value(result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula