#include "xl-formula/functions.h"
#include <chrono>
#include <ctime>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief TIME function - creates a time from hour, minute, second
 * @param args Function arguments (expects 3 numeric arguments: hour, minute, second)
 * @param context Evaluation context (unused for TIME)
 * @return Time value as a fraction of a day (Excel-style)
 */
Value time_function(const std::vector<Value>& args, const Context& context) {
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
    
    // Convert arguments to numbers
    if (!args[0].canConvertToNumber() || !args[1].canConvertToNumber() || !args[2].canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    int hour = static_cast<int>(args[0].toNumber());
    int minute = static_cast<int>(args[1].toNumber());
    int second = static_cast<int>(args[2].toNumber());
    
    // Validate ranges
    if (hour < 0 || hour > 23) {
        return Value::error(ErrorType::NUM_ERROR);
    }
    if (minute < 0 || minute > 59) {
        return Value::error(ErrorType::NUM_ERROR);
    }
    if (second < 0 || second > 59) {
        return Value::error(ErrorType::NUM_ERROR);
    }
    
    // Calculate time as fraction of a day (Excel convention)
    // Total seconds in the time
    double total_seconds = hour * 3600.0 + minute * 60.0 + second;
    // Seconds in a day
    double seconds_per_day = 24.0 * 60.0 * 60.0;
    // Fraction of a day
    double time_fraction = total_seconds / seconds_per_day;
    
    return Value(time_fraction);
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula