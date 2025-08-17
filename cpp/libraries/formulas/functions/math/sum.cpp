#include <numeric>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Adds all numeric arguments
 * @ingroup math
 * @param number1 First number (optional)
 * @param number2 Additional numbers (variadic)
 * @code
 * SUM(1,2,3) -> 6
 * @endcode
 */
Value sum(const std::vector<Value>& args, const Context& context) {
    return templates::multiNumericFunction(
            args, context, "SUM", [](const std::vector<double>& nums) {
                if (nums.empty()) {
                    return 0.0;
                }
                return std::accumulate(nums.begin(), nums.end(), 0.0);
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula