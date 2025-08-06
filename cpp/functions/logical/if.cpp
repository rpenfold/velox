#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

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

    // Convert condition to boolean
    bool is_true = false;
    if (condition.isBoolean()) {
        is_true = condition.asBoolean();
    } else if (condition.canConvertToNumber()) {
        is_true = condition.toNumber() != 0.0;
    } else if (condition.isText()) {
        is_true = !condition.asText().empty();
    }

    return is_true ? true_value : false_value;
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula