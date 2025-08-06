#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value irr(const std::vector<Value>& args, const Context& context) {
    return templates::cashFlowFunction(
            args, context, "IRR", 1, [](const std::vector<double>& args) -> Value {
                std::vector<double> cash_flows;
                double guess = 0.1;  // Default guess

                if (args.size() == 2) {
                    // First argument is array of cash flows, second is guess
                    cash_flows.push_back(args[0]);
                    guess = args[1];
                } else {
                    // All arguments are cash flows
                    cash_flows = args;
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
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula