#include "xl-formula/functions.h"
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

// CORREL(array1, array2)
// For now we accept a flat list of values and split in half if two arrays are not provided as true arrays
Value correl(const std::vector<Value>& args, const Context& context) {
    (void)context;

    if (args.size() < 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Flatten numeric values and split into two vectors
    std::vector<double> x, y;
    if (args.size() == 2 && args[0].isArray() && args[1].isArray()) {
        for (const auto& v : args[0].asArray()) if (v.canConvertToNumber()) x.push_back(v.toNumber());
        for (const auto& v : args[1].asArray()) if (v.canConvertToNumber()) y.push_back(v.toNumber());
    } else {
        // Split list in half
        size_t mid = args.size() / 2;
        for (size_t i = 0; i < mid; ++i) if (args[i].canConvertToNumber()) x.push_back(args[i].toNumber());
        for (size_t i = mid; i < args.size(); ++i) if (args[i].canConvertToNumber()) y.push_back(args[i].toNumber());
    }

    size_t n = std::min(x.size(), y.size());
    if (n < 2) return Value::error(ErrorType::DIV_ZERO);

    // Compute means
    double sx = 0.0, sy = 0.0;
    for (size_t i = 0; i < n; ++i) { sx += x[i]; sy += y[i]; }
    double mx = sx / static_cast<double>(n);
    double my = sy / static_cast<double>(n);

    // Compute covariance and variances
    double sxy = 0.0, sxx = 0.0, syy = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double dx = x[i] - mx;
        double dy = y[i] - my;
        sxy += dx * dy;
        sxx += dx * dx;
        syy += dy * dy;
    }

    if (sxx == 0.0 || syy == 0.0) return Value::error(ErrorType::DIV_ZERO);
    double r = sxy / std::sqrt(sxx * syy);
    return Value(r);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


