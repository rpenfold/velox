#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value edate(const std::vector<Value>& args, const Context& context) {
    (void)context;

    // Validate arg count
    auto error = utils::validateArgCount(args, 2, "EDATE");
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

    auto monthsVal = utils::toNumberSafe(args[1], "EDATE");
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
        int day = tm.tm_mday;

        int totalMonths = (year * 12 + (month - 1)) + months;
        int newYear = totalMonths / 12;
        int newMonth = (totalMonths % 12) + 1; // 1-12

        // Set tm to first of new month to avoid mktime rollovers, then clamp day
        std::tm new_tm{};
        new_tm.tm_year = newYear - 1900;
        new_tm.tm_mon = newMonth - 1;
        new_tm.tm_mday = 1;
        new_tm.tm_hour = 0;
        new_tm.tm_min = 0;
        new_tm.tm_sec = 0;
        new_tm.tm_isdst = -1;

        std::time_t base_t = std::mktime(&new_tm);
        if (base_t == -1) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        // Compute days in target month by going to next month day 0
        std::tm end_of_month_tm = new_tm;
        end_of_month_tm.tm_mon += 1;
        end_of_month_tm.tm_mday = 0; // day 0 of next month -> last day of target month
        std::mktime(&end_of_month_tm);
        int days_in_month = end_of_month_tm.tm_mday;

        new_tm.tm_mday = day > days_in_month ? days_in_month : day;
        std::time_t result_t = std::mktime(&new_tm);
        if (result_t == -1) {
            return Value::error(ErrorType::VALUE_ERROR);
        }

        auto result_tp = std::chrono::system_clock::from_time_t(result_t);
        return Value(result_tp);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula