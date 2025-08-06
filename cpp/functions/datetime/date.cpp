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

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula