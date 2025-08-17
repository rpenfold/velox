#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Returns one value if condition is TRUE, another if FALSE
 * @ingroup logical
 * @param logical_test Condition to evaluate
 * @param value_if_true Value to return if condition is TRUE
 * @param value_if_false Value to return if condition is FALSE
 * @code
 * IF(5>3,"Yes","No") -> "Yes"
 * @endcode
 */
Value if_function(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    auto validation = utils::validateArgCount(args, 3, "IF");
    if (!validation.isEmpty()) {
        return validation;
    }

    const auto& condition = args[0];
    const auto& true_value = args[1];
    const auto& false_value = args[2];

    if (condition.isError())
        return condition;
    if (true_value.isError())
        return true_value;
    if (false_value.isError())
        return false_value;

    // Use shared boolean conversion utility
    bool is_true = ::xl_formula::conditional::toBooleanExcel(condition);
    return is_true ? true_value : false_value;
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula