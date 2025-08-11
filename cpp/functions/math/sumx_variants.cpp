#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

// Helpers to accumulate from either arrays or flat inputs
static void collectTwoSeries(const std::vector<Value>& args, std::vector<double>& a, std::vector<double>& b) {
    if (args.size() == 2 && args[0].isArray() && args[1].isArray()) {
        for (const auto& v : args[0].asArray()) if (v.canConvertToNumber()) a.push_back(v.toNumber());
        for (const auto& v : args[1].asArray()) if (v.canConvertToNumber()) b.push_back(v.toNumber());
    } else {
        size_t mid = args.size() / 2;
        for (size_t i = 0; i < mid; ++i) if (args[i].canConvertToNumber()) a.push_back(args[i].toNumber());
        for (size_t i = mid; i < args.size(); ++i) if (args[i].canConvertToNumber()) b.push_back(args[i].toNumber());
    }
}

// SUMX2MY2: sum(x^2 - y^2)
Value sumx2my2(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.size() < 2) return Value::error(ErrorType::VALUE_ERROR);
    std::vector<double> x, y; collectTwoSeries(args, x, y);
    size_t n = std::min(x.size(), y.size());
    double s = 0.0; for (size_t i = 0; i < n; ++i) s += (x[i]*x[i] - y[i]*y[i]);
    return Value(s);
}

// SUMX2PY2: sum(x^2 + y^2)
Value sumx2py2(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.size() < 2) return Value::error(ErrorType::VALUE_ERROR);
    std::vector<double> x, y; collectTwoSeries(args, x, y);
    size_t n = std::min(x.size(), y.size());
    double s = 0.0; for (size_t i = 0; i < n; ++i) s += (x[i]*x[i] + y[i]*y[i]);
    return Value(s);
}

// SUMXMY2: sum((x - y)^2)
Value sumxmy2(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.size() < 2) return Value::error(ErrorType::VALUE_ERROR);
    std::vector<double> x, y; collectTwoSeries(args, x, y);
    size_t n = std::min(x.size(), y.size());
    double s = 0.0; for (size_t i = 0; i < n; ++i) { double d = x[i]-y[i]; s += d*d; }
    return Value(s);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula


