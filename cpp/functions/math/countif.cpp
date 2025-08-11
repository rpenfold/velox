#include <string>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Helper function to parse and evaluate criteria
 * @param value The value to test
 * @param criteria The criteria string or value
 * @return true if value matches criteria
 */
bool evaluateCriteria(const Value& value, const Value& criteria) {
    // If criteria is a number, do direct comparison
    if (criteria.isNumber()) {
        if (!value.isNumber()) {
            return false;
        }
        return value.asNumber() == criteria.asNumber();
    }

    // If criteria is a boolean, do direct comparison
    if (criteria.isBoolean()) {
        if (!value.isBoolean()) {
            return false;
        }
        return value.asBoolean() == criteria.asBoolean();
    }

    // If criteria is text, parse for operators
    if (criteria.isText()) {
        std::string criteriaStr = criteria.asText();

        // Handle empty criteria
        if (criteriaStr.empty()) {
            return value.isEmpty() || (value.isText() && value.asText().empty());
        }

        // Check for comparison operators
        if (criteriaStr.size() >= 2) {
            std::string op = criteriaStr.substr(0, 2);
            if (op == ">=" || op == "<=" || op == "<>") {
                std::string valueStr = criteriaStr.substr(2);
                double criteriaVal;
                try {
                    criteriaVal = std::stod(valueStr);
                } catch (...) {
                    // If not a number, do text comparison
                    if (!value.isText()) {
                        return false;
                    }
                    std::string valueText = value.asText();
                    if (op == ">=")
                        return valueText >= valueStr;
                    if (op == "<=")
                        return valueText <= valueStr;
                    if (op == "<>")
                        return valueText != valueStr;
                    return false;
                }

                if (!value.isNumber()) {
                    return false;
                }
                double val = value.asNumber();
                if (op == ">=")
                    return val >= criteriaVal;
                if (op == "<=")
                    return val <= criteriaVal;
                if (op == "<>")
                    return val != criteriaVal;
            }
        }

        if (criteriaStr.size() >= 1) {
            char op = criteriaStr[0];
            if (op == '>' || op == '<' || op == '=') {
                std::string valueStr = criteriaStr.substr(1);
                double criteriaVal;
                try {
                    criteriaVal = std::stod(valueStr);
                } catch (...) {
                    // If not a number, do text comparison
                    if (!value.isText()) {
                        return false;
                    }
                    std::string valueText = value.asText();
                    if (op == '>')
                        return valueText > valueStr;
                    if (op == '<')
                        return valueText < valueStr;
                    if (op == '=')
                        return valueText == valueStr;
                    return false;
                }

                if (!value.isNumber()) {
                    return false;
                }
                double val = value.asNumber();
                if (op == '>')
                    return val > criteriaVal;
                if (op == '<')
                    return val < criteriaVal;
                if (op == '=')
                    return val == criteriaVal;
            }
        }

        // Handle wildcards (* and ?) using simpleWildcardMatch to avoid regex bloat
        if (criteriaStr.find('*') != std::string::npos ||
            criteriaStr.find('?') != std::string::npos) {
            if (!value.isText()) {
                return false;
            }
            return simpleWildcardMatch(value.asText(), criteriaStr);
        }

        // Excel coerces numeric-looking text criteria. Compare numerically if possible (numbers, booleans, dates)
        try {
            double criteriaVal = std::stod(criteriaStr);
            if (value.canConvertToNumber()) {
                return value.toNumber() == criteriaVal;
            }
        } catch (...) {
            // not numeric, fall through
        }

        // Direct text comparison otherwise
        if (value.isText()) {
            return value.asText() == criteriaStr;
        }
    }

    return false;
}

/**
 * @brief COUNTIF function - counts the number of cells that meet a criterion
 * @param args Function arguments (expects 2+ arguments: range..., criteria)
 * @param context Evaluation context (unused for COUNTIF)
 * @return Count of values that match criteria
 */
Value countif(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }

    // Validate argument count (at least 2 arguments)
    if (args.size() < 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Last argument is criteria, all others are values to test
    Value criteria = args.back();

    int count = 0;

    // Test each value against criteria
    for (size_t i = 0; i < args.size() - 1; ++i) {
        if (evaluateCriteria(args[i], criteria)) {
            count++;
        }
    }

    return Value(static_cast<double>(count));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula