#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

// Use shared criteria evaluation utility

/**
 * @brief Sums values that meet multiple conditions
 * @ingroup math
 * @param sum_range Values to sum
 * @param criteria_range1 First range to test
 * @param criteria1 First condition
 * @param criteria_range2 Additional ranges (optional, variadic)
 * @param criteria2 Additional conditions (optional, variadic)
 * @code
 * SUMIFS(5, 3, "=3") -> 5
 * @endcode
 */
Value sumifs(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Suppress unused parameter warning

    // SUMIFS requires at least 3 arguments and odd number: sum_range, criteria_range1, criteria1,
    // [criteria_range2, criteria2, ...]
    if (args.size() < 3 || args.size() % 2 == 0) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    try {
        const Value& sumRangeArg = args[0];

        if (sumRangeArg.isError()) {
            return sumRangeArg;
        }

        double sum = 0.0;
        bool matchesAllCriteria = true;

        // Check all criteria pairs
        for (size_t i = 1; i < args.size(); i += 2) {
            const Value& criteriaRange = args[i];
            const Value& criteria = args[i + 1];

            if (criteriaRange.isError())
                return criteriaRange;
            if (criteria.isError())
                return criteria;

            // Simple implementation: check if single value meets criteria
            if (!::xl_formula::conditional::evaluateCriteria(criteriaRange, criteria)) {
                matchesAllCriteria = false;
                break;
            }
        }

        // If all criteria match, add to sum
        if (matchesAllCriteria) {
            auto numValue = utils::toNumberSafe(sumRangeArg, "SUMIFS");
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