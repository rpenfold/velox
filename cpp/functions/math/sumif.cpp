#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Helper function to evaluate criteria without regex (avoiding bloat)
 * @param value The value to test
 * @param criteria The criteria string or value
 * @return true if value matches criteria
 * @deprecated Use utils::evaluateCriteria instead
 */
bool evaluateCriteriaCustom(const Value& value, const Value& criteria) {
    // Delegate to shared utility
    return ::xl_formula::conditional::evaluateCriteria(value, criteria);
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

/**
 * @brief Sums values that meet a condition
 * @ingroup math
 * @param range Range to test against the criterion
 * @param criteria Condition to evaluate
 * @param sum_range Values to sum (optional; defaults to range)
 * @code
 * SUMIF(3, "=3", 5) -> 5
 * @endcode
 */
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