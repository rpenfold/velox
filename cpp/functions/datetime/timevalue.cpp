#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

static std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) start++;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;
    return s.substr(start, end - start);
}

static bool parseManualTime(const std::string& input, std::tm& out_tm) {
    std::string s = trim(input);
    // Detect AM/PM (case-insensitive), optional space
    bool has_ampm = false;
    bool is_pm = false;
    if (s.size() >= 2) {
        std::string lower;
        lower.resize(s.size());
        std::transform(s.begin(), s.end(), lower.begin(), [](unsigned char c){ return std::tolower(c); });
        if (lower.size() >= 2 && (lower.compare(lower.size()-2, 2, "am") == 0 || lower.compare(lower.size()-2, 2, "pm") == 0)) {
            has_ampm = true;
            is_pm = (lower.compare(lower.size()-2, 2, "pm") == 0);
            // Remove AM/PM with potential preceding space
            size_t cut = s.size()-2;
            while (cut > 0 && std::isspace(static_cast<unsigned char>(s[cut-1]))) cut--;
            s = trim(s.substr(0, cut));
        }
    }

    // Split by ':'
    int h = 0, m = 0, sec = 0;
    size_t p1 = s.find(':');
    if (p1 == std::string::npos) return false;
    size_t p2 = s.find(':', p1 + 1);
    try {
        h = std::stoi(s.substr(0, p1));
        if (p2 == std::string::npos) {
            m = std::stoi(s.substr(p1 + 1));
        } else {
            m = std::stoi(s.substr(p1 + 1, p2 - (p1 + 1)));
            sec = std::stoi(s.substr(p2 + 1));
        }
    } catch (...) {
        return false;
    }

    if (has_ampm) {
        if (h < 1 || h > 12) return false;
        int hour24 = h % 12 + (is_pm ? 12 : 0);
        h = hour24;
    } else {
        if (h < 0 || h > 23) return false;
    }
    if (m < 0 || m > 59) return false;
    if (sec < 0 || sec > 59) return false;

    std::tm tm{};
    tm.tm_year = 70; tm.tm_mon = 0; tm.tm_mday = 1;
    tm.tm_hour = h; tm.tm_min = m; tm.tm_sec = sec; tm.tm_isdst = -1;
    out_tm = tm;
    return true;
}

static bool parseTimeText(const std::string& text, std::tm& out_tm) {
    // Try more specific formats first and ensure full string is consumed
    const char* formats[] = {"%H:%M:%S", "%I:%M:%S %p", "%H:%M", "%I:%M %p"};
    for (const char* fmt : formats) {
        std::tm tm{};
        std::istringstream ss(text);
        ss >> std::get_time(&tm, fmt);
        if (!ss.fail()) {
            ss >> std::ws;
            if (ss.eof()) {
                tm.tm_mday = 1;
                tm.tm_mon = 0;
                tm.tm_year = 70; // 1970 baseline
                tm.tm_isdst = -1;
                out_tm = tm;
                return true;
            }
        }
    }
    // Manual fallback
    return parseManualTime(text, out_tm);
}

Value timevalue(const std::vector<Value>& args, const Context& context) {
    (void)context;

    auto error = utils::validateArgCount(args, 1, "TIMEVALUE");
    if (!error.isEmpty()) {
        return error;
    }

    auto err = utils::checkForErrors(args);
    if (!err.isEmpty()) {
        return err;
    }

    std::string text = args[0].toString();

    std::tm tm{};
    if (!parseTimeText(text, tm)) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    std::time_t t = std::mktime(&tm);
    if (t == -1) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    std::tm normalized = *std::localtime(&t);

    int seconds = normalized.tm_hour * 3600 + normalized.tm_min * 60 + normalized.tm_sec;
    double fraction = static_cast<double>(seconds) / (24.0 * 3600.0);
    return Value(fraction);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula