#include "xl-formula/functions.h"
#include <chrono>
#include <ctime>
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

/**
 * @brief MINUTE function - returns the minute from a time/date
 * @param args Function arguments (expects 1 date/time or numeric argument)
 * @param context Evaluation context (unused for MINUTE)
 * @return Minute as numeric value (0-59)
 */
Value minute(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter
    
    // Check for errors first
    auto error = utils::checkForErrors(args);
    if (!error.isEmpty()) {
        return error;
    }
    
    // Validate argument count
    if (args.size() != 1) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    if (args[0].isDate()) {
        // Handle date value
        try {
            auto date_val = args[0].asDate();
            auto time_t = std::chrono::system_clock::to_time_t(date_val);
            auto local_tm = *std::localtime(&time_t);
            
            return Value(static_cast<double>(local_tm.tm_min));
        } catch (...) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    } else if (args[0].canConvertToNumber()) {
        // Handle time fraction (Excel-style)
        double time_fraction = args[0].toNumber();
        
        // Extract fractional part (ignore date part)
        time_fraction = time_fraction - std::floor(time_fraction);
        if (time_fraction < 0) time_fraction += 1.0;  // Handle negative values
        
        // Convert to total seconds in the day
        double total_seconds = time_fraction * 24.0 * 60.0 * 60.0;
        int minute_part = static_cast<int>((total_seconds / 60.0)) % 60;
        
        return Value(static_cast<double>(minute_part));
    } else {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

}  // namespace builtin
}  // namespace functions
}  // namespace xl_formula