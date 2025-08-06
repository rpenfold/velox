#include "xl-formula/functions.h"
#include <numeric>

namespace xl_formula {
namespace functions {
namespace builtin {

Value sum(const std::vector<Value>& args, const Context& context) {
    return templates::multiNumericFunction(args, context, "SUM",
        [](const std::vector<double>& nums) {
            if (nums.empty()) {
                return 0.0;
            }
            return std::accumulate(nums.begin(), nums.end(), 0.0);
        });
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula