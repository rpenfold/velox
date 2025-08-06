#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value mirr(const std::vector<Value>& args, const Context& context) {
    return templates::cashFlowFunction(
            args, context, "MIRR", 3, [](const std::vector<double>& args) -> Value {
                if (args.size() < 3) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                // Extract finance rate and reinvestment rate (last two arguments)
                double finance_rate = args[args.size() - 2];
                double reinvest_rate = args[args.size() - 1];

                // Extract cash flows (all but last two arguments)
                std::vector<double> cash_flows;
                for (size_t i = 0; i < args.size() - 2; i++) {
                    cash_flows.push_back(args[i]);
                }

                if (cash_flows.empty()) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                size_t n = cash_flows.size();

                // Calculate present value of negative cash flows (outflows)
                double pv_outflows = 0.0;
                for (size_t i = 0; i < n; i++) {
                    if (cash_flows[i] < 0) {
                        pv_outflows += cash_flows[i] /
                                       std::pow(1.0 + finance_rate, static_cast<double>(i));
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
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula