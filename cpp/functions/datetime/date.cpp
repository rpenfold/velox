#include <chrono>
#include <ctime>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief DATE function - creates a date from year, month, day
 * @param args Function arguments (expects 3 numeric arguments: year, month, day)
 * @param context Evaluation context (unused for DATE)
 * @return Date value created from the given year, month, day
 */
Value date(const std::vector<Value>& args, const Context& context) {
    return templates::threeNumberFunction(
            args, context, "DATE", [](int year, int month, int day) -> Value {
                // Validate ranges
                if (year < 1900 || year > 2099) {
                    throw std::runtime_error("Year out of range");
                }
                if (month < 1 || month > 12) {
                    throw std::runtime_error("Month out of range");
                }
                if (day < 1 || day > 31) {
                    throw std::runtime_error("Day out of range");
                }

                // Create tm structure
                std::tm time_info = {};
                time_info.tm_year = year - 1900;  // tm_year is years since 1900
                time_info.tm_mon = month - 1;     // tm_mon is 0-based
                time_info.tm_mday = day;
                time_info.tm_hour = 0;
                time_info.tm_min = 0;
                time_info.tm_sec = 0;
                time_info.tm_isdst = -1;  // Let mktime determine DST

                // Convert to time_t
                std::time_t time_val = std::mktime(&time_info);
                if (time_val == -1) {
                    throw std::runtime_error("Invalid date");
                }

                // Convert to system_clock::time_point
                auto time_point = std::chrono::system_clock::from_time_t(time_val);
                return Value(time_point);
            });
}

// EDATE(start_date, months)
Value edate(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 2, "EDATE");
    if (!err.isEmpty()) return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty()) return check;
    if (!args[0].isDate()) return Value::error(ErrorType::VALUE_ERROR);
    auto monthsV = utils::toNumberSafe(args[1], "EDATE");
    if (monthsV.isError()) return monthsV;
    int months = static_cast<int>(monthsV.asNumber());

    auto tp = args[0].asDate();
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tmv = *std::localtime(&tt);
    int y = tmv.tm_year + 1900;
    int m = tmv.tm_mon + 1;
    int d = tmv.tm_mday;
    int total = y * 12 + (m - 1) + months;
    int ny = total / 12;
    int nm = total % 12 + 1;
    // clamp day to last day of new month
    static const int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    auto isLeap=[&](int Y){return (Y%4==0 && Y%100!=0) || (Y%400==0);};
    int last = mdays[nm-1] + ((nm==2 && isLeap(ny))?1:0);
    if (d > last) d = last;
    std::tm out{}; out.tm_year = ny-1900; out.tm_mon = nm-1; out.tm_mday = d;
    auto out_tt = std::mktime(&out);
    return Value(std::chrono::system_clock::from_time_t(out_tt));
}

// EOMONTH(start_date, months)
Value eomonth(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 2, "EOMONTH");
    if (!err.isEmpty()) return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty()) return check;
    if (!args[0].isDate()) return Value::error(ErrorType::VALUE_ERROR);
    auto monthsV = utils::toNumberSafe(args[1], "EOMONTH");
    if (monthsV.isError()) return monthsV;
    int months = static_cast<int>(monthsV.asNumber());

    auto tp = args[0].asDate();
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tmv = *std::localtime(&tt);
    int y = tmv.tm_year + 1900;
    int m = tmv.tm_mon + 1;
    int total = y * 12 + (m - 1) + months;
    int ny = total / 12;
    int nm = total % 12 + 1;
    static const int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    auto isLeap=[&](int Y){return (Y%4==0 && Y%100!=0) || (Y%400==0);};
    int last = mdays[nm-1] + ((nm==2 && isLeap(ny))?1:0);
    std::tm out{}; out.tm_year = ny-1900; out.tm_mon = nm-1; out.tm_mday = last;
    auto out_tt = std::mktime(&out);
    return Value(std::chrono::system_clock::from_time_t(out_tt));
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula