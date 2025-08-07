#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

// Forward declaration from sumif.cpp
bool evaluateCriteriaCustom(const Value& value, const Value& criteria);

Value averageif(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Suppress unused parameter warning

    // AVERAGEIF requires 2 or 3 arguments: range, criteria, [average_range]
    if (args.size() < 2 || args.size() > 3) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    try {
        const Value& rangeArg = args[0];
        const Value& criteriaArg = args[1];
        const Value& averageRangeArg = (args.size() == 3) ? args[2] : rangeArg;

        // For now, treat range as single value (full array support would be more complex)
        if (rangeArg.isError())
            return rangeArg;
        if (averageRangeArg.isError())
            return averageRangeArg;

        double sum = 0.0;
        int count = 0;

        // Simple implementation: check if single value meets criteria
        if (evaluateCriteriaCustom(rangeArg, criteriaArg)) {
            auto numValue = utils::toNumberSafe(averageRangeArg, "AVERAGEIF");
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