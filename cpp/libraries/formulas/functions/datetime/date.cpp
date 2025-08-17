#include <chrono>
#include <ctime>
#include <sstream>
#include "velox/formulas/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief Creates a date from year, month, and day
 * @ingroup datetime
 * @param year Year value
 * @param month Month value (1-12)
 * @param day Day value (1-31)
 * @code
 * DATE(2024,1,15) -> 2024-01-15 00:00:00
 * @endcode
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

/**
 * @brief Returns the date n months before/after start_date
 * @ingroup datetime
 * @param start_date Starting date
 * @param months Number of months before (negative) or after (positive)
 * @code
 * EDATE(DATE(2024,1,31), 1) -> 2024-02-29 00:00:00
 * @endcode
 */
Value edate(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 2, "EDATE");
    if (!err.isEmpty())
        return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty())
        return check;
    if (!args[0].isDate())
        return Value::error(ErrorType::VALUE_ERROR);
    auto monthsV = utils::toNumberSafe(args[1], "EDATE");
    if (monthsV.isError())
        return monthsV;
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
    static const int mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    auto isLeap = [&](int Y) { return (Y % 4 == 0 && Y % 100 != 0) || (Y % 400 == 0); };
    int last = mdays[nm - 1] + ((nm == 2 && isLeap(ny)) ? 1 : 0);
    if (d > last)
        d = last;
    std::tm out{};
    out.tm_year = ny - 1900;
    out.tm_mon = nm - 1;
    out.tm_mday = d;
    auto out_tt = std::mktime(&out);
    return Value(std::chrono::system_clock::from_time_t(out_tt));
}

/**
 * @brief Returns the last day of the month n months before/after start_date
 * @ingroup datetime
 * @param start_date Starting date
 * @param months Number of months before (negative) or after (positive)
 * @code
 * EOMONTH(DATE(2024,1,15), 1) -> 2024-02-29 00:00:00
 * @endcode
 */
Value eomonth(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 2, "EOMONTH");
    if (!err.isEmpty())
        return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty())
        return check;
    if (!args[0].isDate())
        return Value::error(ErrorType::VALUE_ERROR);
    auto monthsV = utils::toNumberSafe(args[1], "EOMONTH");
    if (monthsV.isError())
        return monthsV;
    int months = static_cast<int>(monthsV.asNumber());

    auto tp = args[0].asDate();
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tmv = *std::localtime(&tt);
    int y = tmv.tm_year + 1900;
    int m = tmv.tm_mon + 1;
    int total = y * 12 + (m - 1) + months;
    int ny = total / 12;
    int nm = total % 12 + 1;
    static const int mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    auto isLeap = [&](int Y) { return (Y % 4 == 0 && Y % 100 != 0) || (Y % 400 == 0); };
    int last = mdays[nm - 1] + ((nm == 2 && isLeap(ny)) ? 1 : 0);
    std::tm out{};
    out.tm_year = ny - 1900;
    out.tm_mon = nm - 1;
    out.tm_mday = last;
    auto out_tt = std::mktime(&out);
    return Value(std::chrono::system_clock::from_time_t(out_tt));
}

static inline std::string trim_copy(const std::string& input) {
    size_t start = 0;
    while (start < input.size() && std::isspace(static_cast<unsigned char>(input[start])))
        start++;
    size_t end = input.size();
    while (end > start && std::isspace(static_cast<unsigned char>(input[end - 1])))
        end--;
    return input.substr(start, end - start);
}

/**
 * @brief Converts a date in text to a date value
 * @name DATEVALUE
 * @category datetime
 * @param date_text Date string (e.g., 2024-01-15)
 * @code
 * DATEVALUE("2024-01-15") -> 2024-01-15 00:00:00
 * @endcode
 */
// DATEVALUE(text or date)
Value datevalue(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 1, "DATEVALUE");
    if (!err.isEmpty())
        return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty())
        return check;
    if (args[0].isDate()) {
        // Already a date; return as-is
        return args[0];
    }
    std::string s = trim_copy(args[0].toString());
    if (s.empty())
        return Value::error(ErrorType::VALUE_ERROR);
    // Parse common forms:
    // 1) YYYY-MM-DD or YYYY/MM/DD (allow single-digit M and D)
    // 2) M/D/YYYY (US style) and D-M-YYYY (when using '-')
    int part1 = 0, part2 = 0, part3 = 0;
    char sep1 = 0, sep2 = 0;
    {
        std::istringstream iss(s);
        if (!(iss >> part1))
            return Value::error(ErrorType::VALUE_ERROR);
        if (!(iss >> sep1))
            return Value::error(ErrorType::VALUE_ERROR);
        if (!(iss >> part2))
            return Value::error(ErrorType::VALUE_ERROR);
        if (!(iss >> sep2))
            return Value::error(ErrorType::VALUE_ERROR);
        if (!(iss >> part3))
            return Value::error(ErrorType::VALUE_ERROR);
        if (!((sep1 == '-' || sep1 == '/') && (sep2 == '-' || sep2 == '/'))) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    }
    int y = 0, m = 0, d = 0;
    auto isYear = [](int v) { return v >= 1000 && v <= 9999; };
    if (isYear(part1)) {
        // YYYY-sep-MM-sep-DD
        y = part1;
        m = part2;
        d = part3;
    } else if (isYear(part3)) {
        // Infer order based on first separator: '/' => M/D/YYYY, '-' => D-M-YYYY
        if (sep1 == '/') {
            m = part1;
            d = part2;
            y = part3;
        } else {
            d = part1;
            m = part2;
            y = part3;
        }
    } else {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    if (m < 1 || m > 12 || d < 1 || d > 31)
        return Value::error(ErrorType::VALUE_ERROR);
    std::tm out{};
    out.tm_year = y - 1900;
    out.tm_mon = m - 1;
    out.tm_mday = d;
    out.tm_hour = 0;
    out.tm_min = 0;
    out.tm_sec = 0;
    out.tm_isdst = -1;
    auto out_tt = std::mktime(&out);
    if (out_tt == -1)
        return Value::error(ErrorType::VALUE_ERROR);
    return Value(std::chrono::system_clock::from_time_t(out_tt));
}

/**
 * @brief Converts a time in text to a fraction of a day
 * @name TIMEVALUE
 * @category datetime
 * @param time_text Time string (e.g., 14:30:00 or 1:30 PM)
 * @code
 * TIMEVALUE("12:00:00") -> 0.5
 * @endcode
 */
// TIMEVALUE(text or date)
Value timevalue(const std::vector<Value>& args, const Context& context) {
    (void)context;
    auto err = utils::validateArgCount(args, 1, "TIMEVALUE");
    if (!err.isEmpty())
        return err;
    auto check = utils::checkForErrors(args);
    if (!check.isEmpty())
        return check;
    if (args[0].isDate()) {
        auto tp = args[0].asDate();
        std::time_t tt = std::chrono::system_clock::to_time_t(tp);
        std::tm tmv = *std::localtime(&tt);
        double seconds = tmv.tm_hour * 3600.0 + tmv.tm_min * 60.0 + tmv.tm_sec;
        return Value(seconds / (24.0 * 3600.0));
    }
    std::string sraw = trim_copy(args[0].toString());
    if (sraw.empty())
        return Value::error(ErrorType::VALUE_ERROR);
    // Handle optional AM/PM suffix
    std::string s = sraw;
    bool hasAmPm = false;
    bool isPM = false;
    if (s.size() >= 2) {
        // Normalize to uppercase
        std::string up;
        up.reserve(s.size());
        for (char ch : s)
            up.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));
        auto trim_end = [](std::string& str) {
            while (!str.empty() && std::isspace(static_cast<unsigned char>(str.back())))
                str.pop_back();
        };
        trim_end(up);
        if (up.size() >= 2) {
            if (up.size() >= 3 && up.substr(up.size() - 3) == " AM") {
                hasAmPm = true;
                isPM = false;
                s = s.substr(0, s.size() - 3);
            } else if (up.size() >= 3 && up.substr(up.size() - 3) == " PM") {
                hasAmPm = true;
                isPM = true;
                s = s.substr(0, s.size() - 3);
            } else if (up.substr(up.size() - 2) == "AM") {
                hasAmPm = true;
                isPM = false;
                s = s.substr(0, s.size() - 2);
            } else if (up.substr(up.size() - 2) == "PM") {
                hasAmPm = true;
                isPM = true;
                s = s.substr(0, s.size() - 2);
            }
        }
        s = trim_copy(s);
    }
    int hh = 0, mm = 0, ss = 0;
    char c1 = 0, c2 = 0;
    std::istringstream iss(s);
    if (!(iss >> hh >> c1 >> mm) || c1 != ':') {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    if (iss >> c2) {
        if (c2 == ':') {
            if (!(iss >> ss))
                return Value::error(ErrorType::VALUE_ERROR);
        } else {
            // Unexpected extra content
            return Value::error(ErrorType::VALUE_ERROR);
        }
    }
    if (hasAmPm) {
        if (hh < 1 || hh > 12)
            return Value::error(ErrorType::VALUE_ERROR);
        if (isPM) {
            if (hh != 12)
                hh += 12;
        } else {
            if (hh == 12)
                hh = 0;
        }
    }
    if (hh < 0 || hh > 23 || mm < 0 || mm > 59 || ss < 0 || ss > 59)
        return Value::error(ErrorType::VALUE_ERROR);
    double fraction = (hh * 3600.0 + mm * 60.0 + ss) / (24.0 * 3600.0);
    return Value(fraction);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula