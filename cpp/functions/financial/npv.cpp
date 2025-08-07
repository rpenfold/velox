#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value npv(const std::vector<Value>& args, const Context& context) {
    (void)context;

    if (args.size() < 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    // First argument is always the discount rate
    auto rate_val = utils::toNumberSafe(args[0], "NPV");
    if (rate_val.isError())
        return rate_val;
    double rate = rate_val.asNumber();

    std::vector<double> cash_flows;

    if (args.size() == 2 && args[1].isArray()) {
        // Array syntax: NPV(rate, {cash_flows})
        const auto& cash_flows_array = args[1].asArray();
        for (const auto& val : cash_flows_array) {
            if (val.isError())
                return val;
            auto num = utils::toNumberSafe(val, "NPV");
            if (num.isError())
                return num;
            cash_flows.push_back(num.asNumber());
        }
    } else {
        // Legacy syntax: NPV(rate, cf1, cf2, cf3, ...)
        for (size_t i = 1; i < args.size(); i++) {
            auto num = utils::toNumberSafe(args[i], "NPV");
            if (num.isError())
                return num;
            cash_flows.push_back(num.asNumber());
        }
    }

    // Calculate NPV of cash flows
    double npv_result = 0.0;

    // Process each cash flow (starting from period 1)
    for (size_t i = 0; i < cash_flows.size(); i++) {
        double cash_flow = cash_flows[i];
        double period = static_cast<double>(i + 1);  // Periods start from 1

        // Add discounted cash flow to NPV
        npv_result += cash_flow / std::pow(1.0 + rate, period);
    }

    return Value(npv_result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula