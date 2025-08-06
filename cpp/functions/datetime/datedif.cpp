#include <algorithm>
#include <chrono>
#include <ctime>
#include <string>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief DATEDIF function - calculates the difference between dates
 * @param args Function arguments (expects 3 arguments: start_date, end_date, unit)
 * @param context Evaluation context (unused for DATEDIF)
 * @return Date difference in specified unit
 */
Value datedif(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }

    // Validate argument count
    if (args.size() != 3) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check if first two arguments are dates
    if (!args[0].isDate() || !args[1].isDate()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    // Check if third argument is text (unit)
    if (!args[2].isText()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    try {
        auto start_date = args[0].asDate();
        auto end_date = args[1].asDate();
        std::string unit = args[2].asText();

        // Convert to uppercase for comparison
        std::transform(unit.begin(), unit.end(), unit.begin(), ::toupper);

        // Convert to time_t for easier manipulation
        auto start_time_t = std::chrono::system_clock::to_time_t(start_date);
        auto end_time_t = std::chrono::system_clock::to_time_t(end_date);
        auto start_tm = *std::localtime(&start_time_t);
        auto end_tm = *std::localtime(&end_time_t);

        // Swap if start > end
        if (start_date > end_date) {
            std::swap(start_tm, end_tm);
            std::swap(start_time_t, end_time_t);
        }

        if (unit == "Y") {
            // Years
            int years = end_tm.tm_year - start_tm.tm_year;
            if (end_tm.tm_mon < start_tm.tm_mon ||
                (end_tm.tm_mon == start_tm.tm_mon && end_tm.tm_mday < start_tm.tm_mday)) {
                years--;
            }
            return Value(static_cast<double>(years));
        } else if (unit == "M") {
            // Months
            int months =
                    (end_tm.tm_year - start_tm.tm_year) * 12 + (end_tm.tm_mon - start_tm.tm_mon);
            if (end_tm.tm_mday < start_tm.tm_mday) {
                months--;
            }
            return Value(static_cast<double>(months));
        } else if (unit == "D") {
            // Days
            auto duration = std::chrono::duration_cast<std::chrono::hours>(
                    std::chrono::system_clock::from_time_t(end_time_t) -
                    std::chrono::system_clock::from_time_t(start_time_t));
            return Value(static_cast<double>(duration.count() / 24));
        } else if (unit == "MD") {
            // Days ignoring months and years
            int day_diff = end_tm.tm_mday - start_tm.tm_mday;
            if (day_diff < 0) {
                // Need to borrow from previous month
                std::tm temp_tm = end_tm;
                temp_tm.tm_mon--;
                if (temp_tm.tm_mon < 0) {
                    temp_tm.tm_mon = 11;
                    temp_tm.tm_year--;
                }
                temp_tm.tm_mday = 1;
                auto temp_time = std::mktime(&temp_tm);
                temp_tm = *std::localtime(&temp_time);

                // Days in previous month
                temp_tm.tm_mon++;
                temp_tm.tm_mday = 0;  // Last day of previous month
                auto last_day_time = std::mktime(&temp_tm);
                temp_tm = *std::localtime(&last_day_time);

                day_diff = temp_tm.tm_mday - start_tm.tm_mday + end_tm.tm_mday;
            }
            return Value(static_cast<double>(day_diff));
        } else if (unit == "YM") {
            // Months ignoring years
            int month_diff = end_tm.tm_mon - start_tm.tm_mon;
            if (end_tm.tm_mday < start_tm.tm_mday) {
                month_diff--;
            }
            if (month_diff < 0) {
                month_diff += 12;
            }
            return Value(static_cast<double>(month_diff));
        } else if (unit == "YD") {
            // Days ignoring years
            std::tm temp_start = start_tm;
            temp_start.tm_year = end_tm.tm_year;
            auto temp_start_time = std::mktime(&temp_start);

            if (temp_start_time > end_time_t) {
                temp_start.tm_year--;
                temp_start_time = std::mktime(&temp_start);
            }

            auto duration = std::chrono::duration_cast<std::chrono::hours>(
                    std::chrono::system_clock::from_time_t(end_time_t) -
                    std::chrono::system_clock::from_time_t(temp_start_time));
            return Value(static_cast<double>(duration.count() / 24));
        } else {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula