#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value npv(const std::vector<Value>& args, const Context& context) {
    return templates::cashFlowFunction(
            args, context, "NPV", 2, [](const std::vector<double>& args) -> Value {
                double rate = args[0];

                // Calculate NPV of cash flows
                double npv_result = 0.0;

                // Process each cash flow (starting from period 1)
                for (size_t i = 1; i < args.size(); i++) {
                    double cash_flow = args[i];
                    double period = static_cast<double>(i);

                    // Add discounted cash flow to NPV
                    npv_result += cash_flow / std::pow(1.0 + rate, period);
                }

                return Value(npv_result);
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula