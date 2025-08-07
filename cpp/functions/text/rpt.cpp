#include "xl-formula/functions.h"
#include <string>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief RPT function - repeats text a specified number of times
 * @param args Function arguments (text, repeat_count)
 * @param context Evaluation context (unused for RPT)
 * @return Text repeated the specified number of times
 */
Value rpt(const std::vector<Value>& args, const Context& context) {
    return templates::twoArgTextNumberFunction(
        args, context, "RPT", [](const std::string& text, double repeat_count) {
            // Validate that repeat count is non-negative
            if (repeat_count < 0) {
                return std::string(); // Return empty string for negative counts
            }

            // Convert to integer (truncate decimal part)
            int count = static_cast<int>(repeat_count);

            // Build the repeated string
            std::string result;
            result.reserve(text.length() * count); // Pre-allocate for efficiency

            for (int i = 0; i < count; ++i) {
                result += text;
            }

            return result;
        });
}

}}} // namespace xl_formula::functions::builtin 