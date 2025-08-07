#include <cmath>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value rate(const std::vector<Value>& args, const Context& context) {
    return templates::financialFunction(
            args, context, "RATE", 3, 6, [](const std::vector<double>& args) -> Value {
                double periods = args[0];
                double payment = args[1];
                double present = args[2];
                double future = (args.size() > 3) ? args[3] : 0.0;
                double type = (args.size() > 4) ? args[4] : 0.0;
                double guess = (args.size() > 5) ? args[5] : 0.01;

                // Type should be 0 (end of period) or 1 (beginning of period)
                if (type != 0.0 && type != 1.0) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                // Set maximum epsilon for end of iteration
                const double epsMax = 1e-6;

                // Set maximum number of iterations
                const int iterMax = 100;
                int iter = 0;
                bool close = false;
                double rate = guess;

                while (iter < iterMax && !close) {
                    double f1, f2, f3;

                    if (std::abs(rate) < 1e-10) {
                        // Handle near-zero rate case
                        f1 = future + present + payment * periods;
                        f2 = payment * periods * (periods - 1) / 2.0;
                        f3 = 0.0;
                    } else {
                        const double t1 = std::pow(rate + 1, periods);
                        const double t2 = std::pow(rate + 1, periods - 1);
                        f1 = future + t1 * present + payment * (t1 - 1) * (rate * type + 1) / rate;
                        f2 = periods * t2 * present -
                             payment * (t1 - 1) * (rate * type + 1) / std::pow(rate, 2);
                        f3 = periods * payment * t2 * (rate * type + 1) / rate +
                             payment * (t1 - 1) * type / rate;
                    }

                    const double denominator = f2 + f3;
                    if (std::abs(denominator) < 1e-15) {
                        return Value::error(ErrorType::VALUE_ERROR);
                    }

                    const double newRate = rate - f1 / denominator;

                    // Bound the rate to reasonable values to prevent divergence
                    if (newRate < -0.99 || newRate > 10.0) {
                        return Value::error(ErrorType::VALUE_ERROR);
                    }

                    if (std::abs(newRate - rate) < epsMax) {
                        close = true;
                    }
                    iter++;
                    rate = newRate;
                }

                if (!close) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                return Value(rate);
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula