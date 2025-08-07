#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value rate(const std::vector<Value>& args, const Context& context) {
    return templates::financialFunction(
            args, context, "RATE", 3, 6, [](const std::vector<double>& args) -> Value {
                double nper = args[0];
                double pmt = args[1];
                double pv = args[2];
                double fv = (args.size() > 3) ? args[3] : 0.0;
                double type = (args.size() > 4) ? args[4] : 0.0;
                double guess = (args.size() > 5) ? args[5] : 0.1;

                // Type should be 0 (end of period) or 1 (beginning of period)
                if (type != 0.0 && type != 1.0) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                // Newton-Raphson method to solve for rate
                double rate = guess;
                const double tolerance = 1e-12;
                const int max_iterations = 100;

                for (int i = 0; i < max_iterations; i++) {
                    // Calculate present value using current rate guess
                    double pv_calc, dpv_dr;

                    if (std::abs(rate) < tolerance) {
                        // Handle rate ≈ 0 case
                        pv_calc = -(fv + pmt * nper);
                        dpv_dr = -pmt * nper * (nper - 1) / 2.0;
                    } else {
                        double pvif = std::pow(1.0 + rate, -nper);
                        double pvifa = (1.0 - pvif) / rate;

                        // Present value calculation
                        pv_calc = -(fv * pvif + pmt * pvifa);

                        // Adjust for payment timing
                        if (type == 1.0) {
                            pv_calc = pv_calc - pmt;
                            pvifa += 1.0;
                        }

                        // Derivative of present value with respect to rate
                        double dpvif_dr = -nper * std::pow(1.0 + rate, -nper - 1);
                        double dpvifa_dr = (-dpvif_dr * rate - (1.0 - pvif)) / (rate * rate);

                        dpv_dr = -fv * dpvif_dr - pmt * dpvifa_dr;

                        if (type == 1.0) {
                            dpv_dr -= pmt * dpvifa_dr;
                        }
                    }

                    // Newton-Raphson iteration
                    double f = pv_calc - pv;
                    if (std::abs(f) < tolerance) {
                        return Value(rate);
                    }

                    if (std::abs(dpv_dr) < tolerance) {
                        return Value::error(ErrorType::VALUE_ERROR);
                    }

                    double new_rate = rate - f / dpv_dr;

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