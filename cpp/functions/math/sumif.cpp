#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Helper function to evaluate criteria without regex (avoiding bloat)
 * @param value The value to test
 * @param criteria The criteria string or value
 * @return true if value matches criteria
 */
bool evaluateCriteriaCustom(const Value& value, const Value& criteria) {
    // If criteria is a number, do direct comparison
    if (criteria.isNumber()) {
        if (!value.isNumber()) {
            return false;
        }
        return std::abs(value.asNumber() - criteria.asNumber()) < 1e-10;
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

        // Check for comparison operators (>=, <=, <>, >, <, =)
        if (criteriaStr.size() >= 2) {
            std::string op = criteriaStr.substr(0, 2);
            if (op == ">=" || op == "<=" || op == "<>") {
                std::string valueStr = criteriaStr.substr(2);
                double criteriaVal;
                try {
                    criteriaVal = std::stod(valueStr);
                } catch (...) {
                    // Text comparison
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
                    return std::abs(val - criteriaVal) > 1e-10;
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
                    // Text comparison
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
                    return std::abs(val - criteriaVal) < 1e-10;
            }
        }

        // Simple wildcard matching without regex
        if (criteriaStr.find('*') != std::string::npos ||
            criteriaStr.find('?') != std::string::npos) {
            if (!value.isText()) {
                return false;
            }

            std::string text = value.asText();
            return simpleWildcardMatch(text, criteriaStr);
        }

        // Direct text comparison
        if (value.isText()) {
            return value.asText() == criteriaStr;
        }
    }

    return false;
}

/**
 * @brief Simple wildcard matching without regex
 * @param text The text to match
 * @param pattern Pattern with * and ? wildcards
 * @return true if text matches pattern
 */
bool simpleWildcardMatch(const std::string& text, const std::string& pattern) {
    size_t textPos = 0;
    size_t patternPos = 0;

    while (textPos < text.length() && patternPos < pattern.length()) {
        if (pattern[patternPos] == '*') {
            // Skip consecutive asterisks
            while (patternPos < pattern.length() && pattern[patternPos] == '*') {
                patternPos++;
            }

            // If pattern ends with *, match rest of text
            if (patternPos >= pattern.length()) {
                return true;
            }

            // Find next matching character
            char nextChar = pattern[patternPos];
            while (textPos < text.length() && text[textPos] != nextChar) {
                textPos++;
            }

            if (textPos >= text.length()) {
                return false;
            }
        } else if (pattern[patternPos] == '?' || pattern[patternPos] == text[textPos]) {
            patternPos++;
            textPos++;
        } else {
            return false;
        }
    }

    // Handle remaining asterisks at end of pattern
    while (patternPos < pattern.length() && pattern[patternPos] == '*') {
        patternPos++;
    }

    return textPos == text.length() && patternPos == pattern.length();
}

Value sumif(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Suppress unused parameter warning

    // SUMIF requires 2 or 3 arguments: range, criteria, [sum_range]
    if (args.size() < 2 || args.size() > 3) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    try {
        const Value& rangeArg = args[0];
        const Value& criteriaArg = args[1];
        const Value& sumRangeArg = (args.size() == 3) ? args[2] : rangeArg;

        // For now, treat range as single value (full array support would be more complex)
        if (rangeArg.isError())
            return rangeArg;
        if (sumRangeArg.isError())
            return sumRangeArg;

        double sum = 0.0;

        // Simple implementation: check if single value meets criteria
        if (evaluateCriteriaCustom(rangeArg, criteriaArg)) {
            auto numValue = utils::toNumberSafe(sumRangeArg, "SUMIF");
            if (numValue.isError()) {
                return numValue;
            }
            sum += numValue.asNumber();
        }

        return Value(sum);

    } catch (const std::exception& e) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula