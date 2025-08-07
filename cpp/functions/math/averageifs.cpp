#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

// Forward declaration from sumif.cpp
bool evaluateCriteriaCustom(const Value& value, const Value& criteria);

Value averageifs(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Suppress unused parameter warning

    // AVERAGEIFS requires at least 3 arguments and odd number: average_range, criteria_range1,
    // criteria1, [criteria_range2, criteria2, ...]
    if (args.size() < 3 || args.size() % 2 == 0) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    try {
        const Value& averageRangeArg = args[0];

        if (averageRangeArg.isError()) {
            return averageRangeArg;
        }

        double sum = 0.0;
        int count = 0;
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
            if (!evaluateCriteriaCustom(criteriaRange, criteria)) {
                matchesAllCriteria = false;
                break;
            }
        }

        // If all criteria match, add to average
        if (matchesAllCriteria) {
            auto numValue = utils::toNumberSafe(averageRangeArg, "AVERAGEIFS");
            if (numValue.isError()) {
                return numValue;
            }
            sum += numValue.asNumber();
            count++;
        }

        if (count == 0) {
            return Value::error(ErrorType::DIV_ZERO);
        }

        return Value(sum / count);

    } catch (const std::exception& e) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula