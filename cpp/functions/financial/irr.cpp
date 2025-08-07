#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value irr(const std::vector<Value>& args, const Context& context) {
    (void)context;

    if (args.empty()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check for errors
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }

    std::vector<double> cash_flows;
    double guess = 0.1;  // Default guess

    // First argument must be array of cash flows
    const auto& first_arg = args[0];
    if (first_arg.isArray()) {
        // Extract cash flows from array
        const auto& arr = first_arg.asArray();
        for (const auto& val : arr) {
            if (val.isError())
                return val;
            auto num = utils::toNumberSafe(val, "IRR");
            if (num.isError())
                return num;
            cash_flows.push_back(num.asNumber());
        }
    } else {
        // Legacy support: all arguments are individual cash flows
        // If we have more than 1 arg, the last one might be a guess (optional)
        for (const auto& arg : args) {
            auto num = utils::toNumberSafe(arg, "IRR");
            if (num.isError())
                return num;
            cash_flows.push_back(num.asNumber());
        }

        // Default guess
        guess = 0.1;

        // In legacy mode, if we have multiple args, check if last should be treated as guess
        // We need at least 2 cash flows, so if we have 3+ args, last could be guess
        if (args.size() >= 3) {
            // Try to determine if last argument should be treated as guess
            // For now, we'll assume it is if it's between -1 and 10 (reasonable rate range)
            double potential_guess = cash_flows.back();
            if (potential_guess >= -1.0 && potential_guess <= 10.0) {
                // Check if removing it still leaves us with valid cash flows
                cash_flows.pop_back();
                if (cash_flows.size() >= 2) {
                    guess = potential_guess;
                }
            }
        }
    }

    // If we have 2 args and first is array, second is guess
    if (args.size() == 2 && first_arg.isArray()) {
        auto guess_val = utils::toNumberSafe(args[1], "IRR");
        if (guess_val.isError())
            return guess_val;
        guess = guess_val.asNumber();
    }

    // Need at least one positive and one negative cash flow
    bool has_positive = false, has_negative = false;
    for (double cf : cash_flows) {
        if (cf > 0)
            has_positive = true;
        if (cf < 0)
            has_negative = true;
    }

    if (!has_positive || !has_negative) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Newton-Raphson method to find IRR
    double rate = guess;
    const double tolerance = 1e-12;
    const int max_iterations = 100;

    for (int i = 0; i < max_iterations; i++) {
        double npv = 0.0;
        double dnpv_dr = 0.0;

        // Calculate NPV and its derivative
        for (size_t j = 0; j < cash_flows.size(); j++) {
            double period = static_cast<double>(j);
            double discount_factor = std::pow(1.0 + rate, period);

            npv += cash_flows[j] / discount_factor;

            if (period > 0) {
                dnpv_dr -= cash_flows[j] * period / (discount_factor * (1.0 + rate));
            }
        }

        // Check for convergence
        if (std::abs(npv) < tolerance) {
            return Value(rate);
        }

        // Check for zero derivative
        if (std::abs(dnpv_dr) < tolerance) {
            return Value::error(ErrorType::VALUE_ERROR);
        }

        // Newton-Raphson iteration
        double new_rate = rate - npv / dnpv_dr;

        // Bound the rate to reasonable values
        if (new_rate < -0.99 || new_rate > 10.0) {
            return Value::error(ErrorType::VALUE_ERROR);
        }

        rate = new_rate;
    }

    // If we didn't converge, return an error
    return Value::error(ErrorType::VALUE_ERROR);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula