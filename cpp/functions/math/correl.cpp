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


// RSQ(array_x, array_y) = CORREL^2
Value rsq(const std::vector<Value>& args, const Context& context) {
    auto r = correl(args, context);
    if (r.isError()) return r;
    if (!r.isNumber()) return Value::error(ErrorType::VALUE_ERROR);
    double v = r.asNumber();
    return Value(v * v);
}

// Helpers for regression
static bool extractXY(const std::vector<Value>& args, std::vector<double>& x, std::vector<double>& y) {
    if (args.size() == 2 && args[0].isArray() && args[1].isArray()) {
        for (const auto& v : args[0].asArray()) if (v.canConvertToNumber()) x.push_back(v.toNumber());
        for (const auto& v : args[1].asArray()) if (v.canConvertToNumber()) y.push_back(v.toNumber());
    } else {
        size_t mid = args.size() / 2;
        for (size_t i = 0; i < mid; ++i) if (args[i].canConvertToNumber()) x.push_back(args[i].toNumber());
        for (size_t i = mid; i < args.size(); ++i) if (args[i].canConvertToNumber()) y.push_back(args[i].toNumber());
    }
    size_t n = std::min(x.size(), y.size());
    x.resize(n);
    y.resize(n);
    return n >= 2;
}

Value slope(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.size() < 2) return Value::error(ErrorType::VALUE_ERROR);
    std::vector<double> x, y;
    if (!extractXY(args, x, y)) return Value::error(ErrorType::DIV_ZERO);
    size_t n = x.size();
    double sx=0, sy=0, sxx=0, sxy=0;
    for (size_t i=0;i<n;++i){ sx+=x[i]; sy+=y[i]; sxx+=x[i]*x[i]; sxy+=x[i]*y[i]; }
    double denom = n*sxx - sx*sx;
    if (denom == 0.0) return Value::error(ErrorType::DIV_ZERO);
    double m = (n*sxy - sx*sy)/denom;
    return Value(m);
}

Value intercept(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.size() < 2) return Value::error(ErrorType::VALUE_ERROR);
    std::vector<double> x, y;
    if (!extractXY(args, x, y)) return Value::error(ErrorType::DIV_ZERO);
    size_t n = x.size();
    double sx=0, sy=0, sxx=0, sxy=0;
    for (size_t i=0;i<n;++i){ sx+=x[i]; sy+=y[i]; sxx+=x[i]*x[i]; sxy+=x[i]*y[i]; }
    double denom = n*sxx - sx*sx;
    if (denom == 0.0) return Value::error(ErrorType::DIV_ZERO);
    double m = (n*sxy - sx*sy)/denom;
    double bx = sx/static_cast<double>(n);
    double by = sy/static_cast<double>(n);
    double b = by - m*bx;
    return Value(b);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


