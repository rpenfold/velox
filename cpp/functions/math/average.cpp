#include <xl-formula/functions.h>
#include <numeric>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns the average (arithmetic mean) of the arguments
 * @ingroup math
 * @param number1 First number (optional)
 * @param number2 Additional numbers (variadic)
 * @code
 * AVERAGE(1,2,3,4,5) -> 3
 * @endcode
 */
Value average(const std::vector<Value>& args, const Context& context) {
    // AVERAGE requires at least one argument
    auto validation = utils::validateMinArgs(args, 1, "AVERAGE");
    if (!validation.isEmpty()) {
        return validation;
    }

    return templates::multiNumericFunction(
            args, context, "AVERAGE", [](const std::vector<double>& nums) {
                if (nums.empty()) {
                    throw std::runtime_error("AVERAGE: No numeric values found");
                }
                double sum = std::accumulate(nums.begin(), nums.end(), 0.0);
                return sum / nums.size();
            });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula