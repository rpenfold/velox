#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the number of periods for an investment
 * @ingroup financial
 * @param rate Interest rate per period
 * @param pmt Payment made each period
 * @param pv Present value
 * @param fv Future value (optional)
 * @param type Payment timing: 0=end of period, 1=beginning (optional)
 * @code
 * NPER(0.05, -100, 1000) -> 14.21
 * @endcode
 */
Value nper(const std::vector<Value>& args, const Context& context) {
    return templates::financialFunction(
            args, context, "NPER", 3, 5, [](const std::vector<double>& args) -> Value {
                double rate = args[0];
                double pmt = args[1];
                double pv = args[2];
                double fv = (args.size() > 3) ? args[3] : 0.0;
                double type = (args.size() > 4) ? args[4] : 0.0;

                // Type should be 0 (end of period) or 1 (beginning of period)
                if (type != 0.0 && type != 1.0) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                // Adjust payment for timing
                double adj_pmt = pmt;
                if (type == 1.0) {
                    adj_pmt = pmt * (1.0 + rate);
                }

                // If rate is 0, use simple arithmetic
                if (rate == 0.0) {
                    if (pmt == 0.0) {
                        return Value::error(ErrorType::DIV_ZERO);
                    }
                    return Value(-(pv + fv) / pmt);
                }

                // Standard NPER formula using logarithms
                double numerator = adj_pmt - fv * rate;
                double denominator = adj_pmt + pv * rate;

                // Check for division by zero and invalid log arguments
                if (denominator == 0.0) {
                    return Value::error(ErrorType::DIV_ZERO);
                }

                double ratio = numerator / denominator;
                if (ratio <= 0.0) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                double nper_result = std::log(ratio) / std::log(1.0 + rate);

                return Value(nper_result);
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula