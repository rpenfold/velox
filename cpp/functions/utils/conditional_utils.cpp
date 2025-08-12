#include "xl-formula/conditional_utils.h"
#include <algorithm>
#include <string>
#include "xl-formula/functions.h"

// Local implementation of simpleWildcardMatch to avoid external dependency
namespace {
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
}  // anonymous namespace

namespace xl_formula {
namespace conditional {

bool toBooleanExcel(const Value& value) {
    if (value.isBoolean()) {
        return value.asBoolean();
    } else if (value.canConvertToNumber()) {
        return value.toNumber() != 0.0;
    } else if (value.isText()) {
        return !value.asText().empty();
    }
    return false;
}

bool evaluateCriteria(const Value& value, const Value& criteria) {
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
                    return val != criteriaVal;
            }
        }

        // Single character operators
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
                    return val == criteriaVal;
            }
        }

        // Handle wildcards (* and ?) using simpleWildcardMatch
        if (criteriaStr.find('*') != std::string::npos ||
            criteriaStr.find('?') != std::string::npos) {
            if (!value.isText()) {
                return false;
            }
            return simpleWildcardMatch(value.asText(), criteriaStr);
        }

        // Direct text comparison
        if (value.isText()) {
            return value.asText() == criteriaStr;
        }

        // Try to convert text criteria to number and compare
        try {
            double criteriaVal = std::stod(criteriaStr);
            if (value.isNumber()) {
                return value.asNumber() == criteriaVal;
            }
        } catch (...) {
            // Not a number, fall through
        }
    }

    return false;
}

bool evaluateAllCriteria(const std::vector<Value>& args, size_t start_index) {
    // Check all criteria pairs starting from start_index
    for (size_t i = start_index; i < args.size(); i += 2) {
        if (i + 1 >= args.size()) {
            return false;  // Incomplete pair
        }

        const Value& criteriaRange = args[i];
        const Value& criteria = args[i + 1];

        if (criteriaRange.isError() || criteria.isError()) {
            return false;
        }

        if (!evaluateCriteria(criteriaRange, criteria)) {
            return false;
        }
    }
    return true;
}

}  // namespace conditional
}  // namespace xl_formula