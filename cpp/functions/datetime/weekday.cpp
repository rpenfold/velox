#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief WEEKDAY function - returns the day of the week
 * @param args Function arguments (expects 1 date argument, optional return_type)
 * @param context Evaluation context (unused for WEEKDAY)
 * @return Day of the week as numeric value
 */
Value weekday(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }

    // Validate argument count (1 or 2 arguments)
    if (args.size() < 1 || args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check if first argument is a date
    if (!args[0].isDate()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Get return type (default to 1)
    int return_type = 1;
    if (args.size() == 2) {
        if (!args[1].canConvertToNumber()) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        return_type = static_cast<int>(args[1].toNumber());
        if (return_type < 1 || return_type > 3) {
            return Value::error(ErrorType::NUM_ERROR);
        }
    }

    try {
        auto date_val = args[0].asDate();
        auto time_t = std::chrono::system_clock::to_time_t(date_val);
        auto local_tm = *std::localtime(&time_t);

        int weekday_val = local_tm.tm_wday;  // 0=Sunday, 1=Monday, ..., 6=Saturday

        switch (return_type) {
            case 1:  // 1=Sunday, 2=Monday, ..., 7=Saturday
                return Value(static_cast<double>(weekday_val + 1));
            case 2:  // 1=Monday, 2=Tuesday, ..., 7=Sunday
                return Value(static_cast<double>(weekday_val == 0 ? 7 : weekday_val));
            case 3:  // 0=Monday, 1=Tuesday, ..., 6=Sunday
                return Value(static_cast<double>((weekday_val + 6) % 7));
            default:
                return Value::error(ErrorType::NUM_ERROR);
        }
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula