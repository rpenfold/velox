#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

static bool parseDateText(const std::string& text, std::tm& out_tm) {
    // Try multiple common formats: YYYY-MM-DD, YYYY/MM/DD, MM/DD/YYYY, DD/MM/YYYY
    const char* formats[] = {"%Y-%m-%d", "%Y/%m/%d", "%m/%d/%Y", "%d/%m/%Y", "%Y.%m.%d"};
    for (const char* fmt : formats) {
        std::tm tm{};
        std::istringstream ss(text);
        ss >> std::get_time(&tm, fmt);
        if (!ss.fail()) {
            // Normalize fields
            tm.tm_hour = 0; tm.tm_min = 0; tm.tm_sec = 0; tm.tm_isdst = -1;
            out_tm = tm;
            return true;
        }
    }
    return false;
}

Value datevalue(const std::vector<Value>& args, const Context& context) {
    (void)context;

    auto error = utils::validateArgCount(args, 1, "DATEVALUE");
    if (!error.isEmpty()) {
        return error;
    }

    auto err = utils::checkForErrors(args);
    if (!err.isEmpty()) {
        return err;
    }

    // Accept text; numbers are invalid for DATEVALUE here
    std::string text = args[0].toString();

    std::tm tm{};
    if (!parseDateText(text, tm)) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    std::time_t t = std::mktime(&tm);
    if (t == -1) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    auto tp = std::chrono::system_clock::from_time_t(t);
    return Value(tp);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula