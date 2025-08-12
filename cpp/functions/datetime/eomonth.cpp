#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value eomonth(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // Validate arg count
    auto error = utils::validateArgCount(args, 2, "EOMONTH");
    if (!error.isEmpty()) {
        return error;
    }

    // Propagate existing errors
    auto err = utils::checkForErrors(args);
    if (!err.isEmpty()) {
        return err;
    }

    // First arg must be a date
    if (!args[0].isDate()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    auto monthsVal = utils::toNumberSafe(args[1], "EOMONTH");
    if (monthsVal.isError()) {
        return monthsVal;
    }

    int months = static_cast<int>(monthsVal.asNumber());

    try {
        auto start = args[0].asDate();
        std::time_t tt = std::chrono::system_clock::to_time_t(start);
        std::tm tm = *std::localtime(&tt);

        int year = tm.tm_year + 1900;
        int month = tm.tm_mon + 1; // 1-12

        int totalMonths = (year * 12 + (month - 1)) + months;
        int newYear = totalMonths / 12;
        int newMonth = (totalMonths % 12) + 1; // 1-12

        std::tm new_tm{};
        new_tm.tm_year = newYear - 1900;
        new_tm.tm_mon = newMonth - 1;
        new_tm.tm_mday = 1;
        new_tm.tm_hour = 0;
        new_tm.tm_min = 0;
        new_tm.tm_sec = 0;
        new_tm.tm_isdst = -1;

        // Move to day 0 of next month to get end of desired month
        std::tm end_tm = new_tm;
        end_tm.tm_mon += 1;
        end_tm.tm_mday = 0;
        std::time_t end_t = std::mktime(&end_tm);
        if (end_t == -1) {
            return Value::error(ErrorType::VALUE_ERROR);
        }

        auto result_tp = std::chrono::system_clock::from_time_t(end_t);
        return Value(result_tp);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula