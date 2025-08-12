#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the modified internal rate of return for a series of cash flows
 * @ingroup financial
 * @param values Cash flow values (array or arguments)
 * @param finance_rate Interest rate for financing costs
 * @param reinvest_rate Interest rate for reinvestment
 * @code
 * MIRR({-1000, 300, 400, 500}, 0.1, 0.12) -> 0.1342
 * @endcode
 */
Value mirr(const std::vector<Value>& args, const Context& context) {
    (void)context;

    if (args.size() < 3) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::vector<double> cash_flows;
    double finance_rate, reinvest_rate;

    if (args.size() == 3 && args[0].isArray()) {
        // Array syntax: MIRR({cash_flows}, finance_rate, reinvest_rate)
        const auto& cash_flows_array = args[0].asArray();
        for (const auto& val : cash_flows_array) {
            if (val.isError())
                return val;
            auto num = utils::toNumberSafe(val, "MIRR");
            if (num.isError())
                return num;
            cash_flows.push_back(num.asNumber());
        }

        auto finance_rate_val = utils::toNumberSafe(args[1], "MIRR");
        if (finance_rate_val.isError())
            return finance_rate_val;
        finance_rate = finance_rate_val.asNumber();

        auto reinvest_rate_val = utils::toNumberSafe(args[2], "MIRR");
        if (reinvest_rate_val.isError())
            return reinvest_rate_val;
        reinvest_rate = reinvest_rate_val.asNumber();
    } else {
        // Legacy syntax: MIRR(cf1, cf2, cf3, ..., finance_rate, reinvest_rate)
        // Last two arguments are rates, rest are cash flows
        if (args.size() < 3) {
            return Value::error(ErrorType::VALUE_ERROR);
        }

        // Extract cash flows (all but last two arguments)
        for (size_t i = 0; i < args.size() - 2; i++) {
            auto num = utils::toNumberSafe(args[i], "MIRR");
            if (num.isError())
                return num;
            cash_flows.push_back(num.asNumber());
        }

        // Last two arguments are rates
        auto finance_rate_val = utils::toNumberSafe(args[args.size() - 2], "MIRR");
        if (finance_rate_val.isError())
            return finance_rate_val;
        finance_rate = finance_rate_val.asNumber();

        auto reinvest_rate_val = utils::toNumberSafe(args[args.size() - 1], "MIRR");
        if (reinvest_rate_val.isError())
            return reinvest_rate_val;
        reinvest_rate = reinvest_rate_val.asNumber();
    }

    if (cash_flows.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    size_t n = cash_flows.size();

    // Calculate present value of negative cash flows (outflows)
    double pv_outflows = 0.0;
    for (size_t i = 0; i < n; i++) {
        if (cash_flows[i] < 0) {
            pv_outflows += cash_flows[i] / std::pow(1.0 + finance_rate, static_cast<double>(i));
        }
    }

    // Calculate future value of positive cash flows (inflows)
    double fv_inflows = 0.0;
    for (size_t i = 0; i < n; i++) {
        if (cash_flows[i] > 0) {
            double periods_to_end = static_cast<double>(n - 1 - i);
            fv_inflows += cash_flows[i] * std::pow(1.0 + reinvest_rate, periods_to_end);
        }
    }

    // Check for valid calculations
    if (pv_outflows == 0.0 || fv_inflows == 0.0) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Calculate MIRR
    double mirr_result = std::pow(-fv_inflows / pv_outflows, 1.0 / (n - 1)) - 1.0;

    return Value(mirr_result);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula