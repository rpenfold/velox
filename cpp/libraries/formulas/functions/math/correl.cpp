#include <cmath>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Pearson correlation coefficient between two data sets
 * @ingroup math
 * @name CORREL
 * @param array1 First range or array
 * @param array2 Second range or array
 * @see PEARSON
 * @code
 * CORREL([1,2,3,4],[2,4,6,8]) -> 1
 * @endcode
 */
// For now we accept a flat list of values and split in half if two arrays are not provided as true
// arrays
Value correl(const std::vector<Value>& args, const Context& context) {
    (void)context;

    if (args.size() < 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Flatten numeric values and split into two vectors
    std::vector<double> x, y;
    if (args.size() == 2 && args[0].isArray() && args[1].isArray()) {
        for (const auto& v : args[0].asArray())
            if (v.canConvertToNumber())
                x.push_back(v.toNumber());
        for (const auto& v : args[1].asArray())
            if (v.canConvertToNumber())
                y.push_back(v.toNumber());
    } else {
        // Split list in half
        size_t mid = args.size() / 2;
        for (size_t i = 0; i < mid; ++i)
            if (args[i].canConvertToNumber())
                x.push_back(args[i].toNumber());
        for (size_t i = mid; i < args.size(); ++i)
            if (args[i].canConvertToNumber())
                y.push_back(args[i].toNumber());
    }

    size_t n = std::min(x.size(), y.size());
    if (n < 2)
        return Value::error(ErrorType::DIV_ZERO);

    // Compute means
    double sx = 0.0, sy = 0.0;
    for (size_t i = 0; i < n; ++i) {
        sx += x[i];
        sy += y[i];
    }
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

    if (sxx == 0.0 || syy == 0.0)
        return Value::error(ErrorType::DIV_ZERO);
    double r = sxy / std::sqrt(sxx * syy);
    return Value(r);
}

/**
 * @brief Returns the square of the Pearson correlation coefficient (R^2)
 * @ingroup math
 * @param array_x First range or array
 * @param array_y Second range or array
 * @code
 * RSQ([1,2,3,4],[2,4,6,8]) -> 1
 * @endcode
 */
Value rsq(const std::vector<Value>& args, const Context& context) {
    auto r = correl(args, context);
    if (r.isError())
        return r;
    if (!r.isNumber())
        return Value::error(ErrorType::VALUE_ERROR);
    double v = r.asNumber();
    return Value(v * v);
}

// Helpers for regression
static bool extractXY(const std::vector<Value>& args, std::vector<double>& x,
                      std::vector<double>& y) {
    if (args.size() == 2 && args[0].isArray() && args[1].isArray()) {
        for (const auto& v : args[0].asArray())
            if (v.canConvertToNumber())
                x.push_back(v.toNumber());
        for (const auto& v : args[1].asArray())
            if (v.canConvertToNumber())
                y.push_back(v.toNumber());
    } else {
        size_t mid = args.size() / 2;
        for (size_t i = 0; i < mid; ++i)
            if (args[i].canConvertToNumber())
                x.push_back(args[i].toNumber());
        for (size_t i = mid; i < args.size(); ++i)
            if (args[i].canConvertToNumber())
                y.push_back(args[i].toNumber());
    }
    size_t n = std::min(x.size(), y.size());
    x.resize(n);
    y.resize(n);
    return n >= 2;
}

/**
 * @brief Returns the slope of the linear regression line through data points
 * @ingroup math
 * @param known_y’s Dependent data points
 * @param known_x’s Independent data points
 * @code
 * SLOPE([2,4,6,8],[1,2,3,4]) -> 2
 * @endcode
 */
Value slope(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.size() < 2)
        return Value::error(ErrorType::VALUE_ERROR);
    // Excel signature: SLOPE(known_y’s, known_x’s)
    std::vector<double> y, x;
    if (!extractXY({args[1], args[0]}, x, y))
        return Value::error(ErrorType::DIV_ZERO);
    size_t n = x.size();
    double sx = 0, sy = 0, sxx = 0, sxy = 0;
    for (size_t i = 0; i < n; ++i) {
        sx += x[i];
        sy += y[i];
        sxx += x[i] * x[i];
        sxy += x[i] * y[i];
    }
    double denom = n * sxx - sx * sx;
    if (denom == 0.0)
        return Value::error(ErrorType::DIV_ZERO);
    double m = (n * sxy - sx * sy) / denom;
    return Value(m);
}

/**
 * @brief Returns the y-intercept of the linear regression line through data points
 * @ingroup math
 * @param known_y’s Dependent data points
 * @param known_x’s Independent data points
 * @code
 * INTERCEPT([2,4,6,8],[1,2,3,4]) -> 0
 * @endcode
 */
Value intercept(const std::vector<Value>& args, const Context& context) {
    (void)context;
    if (args.size() < 2)
        return Value::error(ErrorType::VALUE_ERROR);
    // Excel signature: INTERCEPT(known_y’s, known_x’s)
    std::vector<double> y, x;
    if (!extractXY({args[1], args[0]}, x, y))
        return Value::error(ErrorType::DIV_ZERO);
    size_t n = x.size();
    double sx = 0, sy = 0, sxx = 0, sxy = 0;
    for (size_t i = 0; i < n; ++i) {
        sx += x[i];
        sy += y[i];
        sxx += x[i] * x[i];
        sxy += x[i] * y[i];
    }
    double denom = n * sxx - sx * sx;
    if (denom == 0.0)
        return Value::error(ErrorType::DIV_ZERO);
    double m = (n * sxy - sx * sy) / denom;
    double bx = sx / static_cast<double>(n);
    double by = sy / static_cast<double>(n);
    double b = by - m * bx;
    return Value(b);
}

/**
 * @brief Historical covariance (population denominator N)
 * @name COVAR
 * @category math
 * @syntax COVAR(array_x, array_y)
 * @param array_x First range or array
 * @param array_y Second range or array
 * @code
 * COVAR([1,2,3],[2,4,6]) -> 1.3333
 * @endcode
 */
// COVAR: historical covariance (population denominator N)
static Value covarianceImpl(const std::vector<Value>& args, const Context& context, bool sample) {
    (void)context;
    if (args.size() < 2)
        return Value::error(ErrorType::VALUE_ERROR);
    std::vector<double> x, y;
    if (!extractXY(args, x, y))
        return Value::error(ErrorType::DIV_ZERO);
    size_t n = x.size();
    double sx = 0, sy = 0;
    for (size_t i = 0; i < n; ++i) {
        sx += x[i];
        sy += y[i];
    }
    double mx = sx / static_cast<double>(n);
    double my = sy / static_cast<double>(n);
    double sxy = 0.0;
    for (size_t i = 0; i < n; ++i) {
        sxy += (x[i] - mx) * (y[i] - my);
    }
    double denom = sample ? (static_cast<double>(n) - 1.0) : static_cast<double>(n);
    if (denom == 0.0)
        return Value::error(ErrorType::DIV_ZERO);
    return Value(sxy / denom);
}

/**
 * @brief Returns population covariance of two data sets (historical; same as COVARIANCE.P)
 * @ingroup math
 * @param array_x First range or array
 * @param array_y Second range or array
 * @code
 * COVAR([1,2,3],[2,4,6]) -> 1.3333
 * @endcode
 */
Value covar(const std::vector<Value>& args, const Context& context) {
    return covarianceImpl(args, context, false);
}
/**
 * @brief Population covariance (denominator N)
 * @name COVARIANCE.P
 * @category math
 * @syntax COVARIANCE.P(array_x, array_y)
 * @param array_x First range or array
 * @param array_y Second range or array
 * @code
 * COVARIANCE.P([1,2,3],[2,4,6]) -> 1.3333
 * @endcode
 */
Value covariance_p(const std::vector<Value>& args, const Context& context) {
    return covarianceImpl(args, context, false);
}
/**
 * @brief Sample covariance (denominator N-1)
 * @name COVARIANCE.S
 * @category math
 * @syntax COVARIANCE.S(array_x, array_y)
 * @param array_x First range or array
 * @param array_y Second range or array
 * @code
 * COVARIANCE.S([1,2,3],[2,4,6]) -> 2
 * @endcode
 */
Value covariance_s(const std::vector<Value>& args, const Context& context) {
    return covarianceImpl(args, context, true);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula
