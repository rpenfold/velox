#include <cmath>
#include <limits>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the tangent of an angle (in radians)
 * @ingroup math
 * @param angle Angle in radians
 * @code
 * TAN(PI()/4) -> 1
 * @endcode
 */
Value tan_function(const std::vector<Value>& args, const Context& context) {
    return templates::singleNumericFunction(args, context, "TAN", [](double x) {
        // Check if x is close to an odd multiple of π/2
        // where tan should be infinity
        double pi_half = M_PI / 2.0;
        double remainder = std::fmod(std::abs(x), M_PI);

        // Check if close to π/2 or 3π/2 (within numerical precision)
        if (std::abs(remainder - pi_half) < 1e-10 || std::abs(remainder - 3.0 * pi_half) < 1e-10) {
            return std::numeric_limits<double>::infinity();
        }

        return std::tan(x);
    });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula