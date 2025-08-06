#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value pmt(const std::vector<Value>& args, const Context& context) {
    return templates::financialFunction(
            args, context, "PMT", 3, 5, [](const std::vector<double>& args) -> Value {
                double rate = args[0];
                double nper = args[1];
                double pv = args[2];
                double fv = (args.size() > 3) ? args[3] : 0.0;
                double type = (args.size() > 4) ? args[4] : 0.0;

                // Type should be 0 (end of period) or 1 (beginning of period)
                if (type != 0.0 && type != 1.0) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                // If rate is 0, use simple arithmetic
                if (rate == 0.0) {
                    return Value(-(pv + fv) / nper);
                }

                // Standard payment formula
                double pvif = std::pow(1.0 + rate, nper);
                double pmt_amount = -(pv * pvif + fv) / ((pvif - 1.0) / rate);

                // Adjust for payment timing
                if (type == 1.0) {
                    pmt_amount = pmt_amount / (1.0 + rate);
                }

                return Value(pmt_amount);
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula