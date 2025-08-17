#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace utils {

Value validateMinArgs(const std::vector<Value>& args, size_t min_count,
                      const std::string& function_name) {
    (void)function_name;  // Unused in this implementation

    if (args.size() < min_count) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    return Value::empty();
}

Value validateArgCount(const std::vector<Value>& args, size_t count,
                       const std::string& function_name) {
    (void)function_name;  // Unused in this implementation

    if (args.size() != count) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    return Value::empty();
}

Value checkForErrors(const std::vector<Value>& args) {
    for (const auto& arg : args) {
        if (arg.isError()) {
            return arg;
        }
    }
    return Value::empty();
}

Value toNumberSafe(const Value& value, const std::string& function_name) {
    (void)function_name;  // Unused in this implementation

    if (value.isError()) {
        return value;
    }

    if (!value.canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    try {
        return Value(value.toNumber());
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace utils
}  // namespace functions
}  // namespace xl_formula