#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the future value of an investment
 * @ingroup financial
 * @param rate Interest rate per period
 * @param nper Number of payment periods
 * @param pmt Payment made each period
 * @param pv Present value (optional)
 * @param type Payment timing: 0=end of period, 1=beginning (optional)
 * @code
 * FV(0.05, 10, -100, 0, 0) -> 1257.79
 * @endcode
 */
Value fv(const std::vector<Value>& args, const Context& context) {
    return templates::financialFunction(
            args, context, "FV", 3, 5, [](const std::vector<double>& args) -> Value {
                double rate = args[0];
                double nper = args[1];
                double pmt = args[2];
                double pv = (args.size() > 3) ? args[3] : 0.0;
                double type = (args.size() > 4) ? args[4] : 0.0;

                // Type should be 0 (end of period) or 1 (beginning of period)
                if (type != 0.0 && type != 1.0) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                // If rate is 0, use simple arithmetic
                if (rate == 0.0) {
                    return Value(-(pv + pmt * nper));
                }

                // Standard future value formula
                double fv_present = pv * std::pow(1.0 + rate, nper);
                double fv_annuity = pmt * (std::pow(1.0 + rate, nper) - 1.0) / rate;

                // Adjust for payment timing
                if (type == 1.0) {
                    fv_annuity *= (1.0 + rate);
                }

                return Value(-(fv_present + fv_annuity));
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula