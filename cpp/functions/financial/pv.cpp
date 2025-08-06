#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value pv(const std::vector<Value>& args, const Context& context) {
    return templates::financialFunction(
            args, context, "PV", 3, 5, [](const std::vector<double>& args) -> Value {
                double rate = args[0];
                double nper = args[1];
                double pmt = args[2];
                double fv = (args.size() > 3) ? args[3] : 0.0;
                double type = (args.size() > 4) ? args[4] : 0.0;

                // Type should be 0 (end of period) or 1 (beginning of period)
                if (type != 0.0 && type != 1.0) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                // If rate is 0, use simple arithmetic
                if (rate == 0.0) {
                    return Value(-(fv + pmt * nper));
                }

                // Standard present value formula
                double pv_annuity = pmt * (1.0 - std::pow(1.0 + rate, -nper)) / rate;
                double pv_future = fv / std::pow(1.0 + rate, nper);

                // Adjust for payment timing
                if (type == 1.0) {
                    pv_annuity *= (1.0 + rate);
                }

                return Value(-(pv_annuity + pv_future));
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula